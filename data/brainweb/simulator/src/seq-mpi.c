/*************************************************************************
* $Id: seq-mpi.c,v 1.7 2006/12/18 15:10:38 yougz Exp $
**************************************************************************
Fonctions that enable the parallelisation of the sequence execution, 
at a level of the sequence itself.

The writing convention for a sequence named 'MySequence' that would
be written in a seq-my.c file,  is the following:

 [...]
 #include <seq_mpi.h>
 [...]

 void MySequence_0(SEQPARAM *seqparam,EXPERIENCE3D* expr);

 PPPVOLUME_COMPLEX_DOUBLE MySequence(SEQPARAM *seqparam,EXPERIENCE3D* expr)
 {
 	mpi_job(seqparam, expr, MySequence_0);
	NormalizeRFSignal(expr); // if needed
	return(GetSignalRFComplexFromExperience(expr));
 }

 void MySequence_0(SEQPARAM *seqparam,EXPERIENCE3D* expr)
 {
 	// write your sequence here
 }
**************************************************************************
 This software is governed by the CeCILL  license under French law and
  abiding by the rules of distribution of free software.  You can  use, 
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info". 
  
  As a counterpart to the access to the source code and  rights to copy,
  modify and redistribute granted by the license, users are provided only
  with a limited warranty  and the software's author,  the holder of the
  economic rights,  and the successive licensors  have only  limited
  liability. 
  
  In this respect, the user's attention is drawn to the risks associated
  with loading,  using,  modifying and/or developing or reproducing the
  software by the user in light of its specific status of free software,
  that may mean  that it is complicated to manipulate,  and  that  also
  therefore means  that it is reserved for developers  and  experienced
  professionals having in-depth computer knowledge. Users are therefore
  encouraged to load and test the software's suitability as regards their
  requirements in conditions enabling the security of their systems and/or 
  data to be ensured and,  more generally, to use and operate it in the 
  same conditions as regards security. 
  
  The fact that you are presently reading this means that you have had
  knowledge of the CeCILL license and that you accept its terms.
  
  Copyright (c) CREATIS (Centre de Recherche et d'Applications en Traitement de
  l'Image). All rights reserved. See License.txt for details.
  
  Version 2.0  20/12/2006
*************************************************************************/
#include "seq-mpi.h"
#include <stdlib.h>

#include "simri3d.h"
#include "experience.h"

#ifdef HAVE_MPI
#include <mpi.h>

#ifdef MPI_DEBUG
/*!
*************************************************************
* \ingroup MPI
* \brief for debugging purpose, print min/max/average of
* the expr->magstate[] table, for
* x values included in the working slice.
*************************************************************/
static void
mpi_context_print_stats (EXPERIENCE3D * expr, int x)
{
    float min1, max1, avg1=0;
    long nb1=0;
    int ix, iy, iz, i;
    float e;
    int slice_width;

    slice_width=mpi_get_slice_width(expr, x);

    printf ("x=%d, ",x);
    /* expr->magstate */
    min1 = max1 = expr->magstate[0][x][0][0];
    for (i=0;i<3;i++)
    for (ix=x; ix<x+slice_width;ix++)
    for (iy=0;iy<expr->nfovy;iy++)
    for (iz=0;iz<expr->nfovz;iz++) {
        e = expr->magstate[i][ix][iy][iz];
        nb1++;
        avg1 += e;
        if (e > max1) max1 = e;
        if (e < min1) min1 = e;
    }
    avg1 = avg1 / nb1;
    printf ("etat->[%.f,%.f,%.f], ",min1,avg1,max1);
}
#endif

/*!
*************************************************************
* \ingroup MPI
* \brief Receive the interesting part of the expr->magstate[] table
* from a given computation node.
*************************************************************/
static void 
mpi_recv_context(EXPERIENCE3D * expr, int x, int src, MPI_Status status)
{
    int ix, iy;
    static int run_nb=0;
    static int buffer_size=0;
    static double *buffer;
    double *index;
    int slice_width;
#ifdef MPI_DEBUG
    int myrank;
#endif

#ifdef MPI_DEBUG
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
#endif

    /*
     * Allocate a contiguous buffer at the first call. This single buffer
     * allows us to issue a single MPI_Send() command, and reduces the
     * required synchronisation between communicating nodes.
     */
    if (run_nb==0) {
	buffer_size=expr->nfovz*3*expr->nfovy*expr->mpi_slice_width;
	buffer = (double *)malloc(buffer_size*sizeof(double));
    }

    /*
     * transmitted buffer size depends of the effective slice width
     */
    slice_width=mpi_get_slice_width(expr, x);
    buffer_size=expr->nfovz*3*expr->nfovy*slice_width;

    MPI_Recv(buffer, buffer_size, MPI_DOUBLE, 
	src, WORKTAG, MPI_COMM_WORLD, &status);

#ifdef MPI_DEBUG
    printf("[node %d]: mpi_recv_context from %d : before recv() ", 
	myrank, status.MPI_SOURCE);
    mpi_context_print_stats (expr,x);
#endif

    index=buffer;
    for (ix=x; ix<x+slice_width; ix++)
	for (iy=0; iy<expr->nfovy; iy++) {
	    memcpy(expr->magstate[0][ix][iy],index,expr->nfovz*sizeof(double));
	    index += expr->nfovz;
	    memcpy(expr->magstate[1][ix][iy],index,expr->nfovz*sizeof(double));
	    index += expr->nfovz;
	    memcpy(expr->magstate[2][ix][iy],index,expr->nfovz*sizeof(double));
	    index += expr->nfovz;
	    }

#ifdef MPI_DEBUG
    printf("[node %d]: mpi_recv_context from %d : after recv() ", 
	myrank, status.MPI_SOURCE);
    mpi_context_print_stats (expr,x);
#endif

    run_nb++;
}

/*!
*************************************************************
* \ingroup MPI
* \brief Receive the signal table from a given computation node, and
* update the node own expr->sgn table.
*************************************************************/
static void
mpi_recv_and_update_signal(EXPERIENCE3D * expr, int src, 
	MPI_Status status)
{
    int ix,iy,iz;
    static int run_nb=0;
    static int buffer_size=0;
    static float *buffer;
    float *index;
#ifdef MPI_DEBUG
    int myrank;
#endif

#ifdef MPI_DEBUG
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
#endif

    /*
     * Allocate a contiguous buffer at the first call. This single buffer
     * allows us to issue a single MPI_Send() command, and reduces the
     * required synchronisation between communicating nodes.
     */
    if (run_nb==0) {
        buffer_size=3*expr->ntx*expr->nty*expr->ntz;
        buffer = (float *)malloc(buffer_size*sizeof(float));
    }

#ifdef MPI_DEBUG
    printf("[node %d]: mpi_recv_signal from %d : before recv() ", 
	myrank, status.MPI_SOURCE);
#endif
    MPI_Recv(buffer, buffer_size, MPI_FLOAT,
    	src, WORKTAG, MPI_COMM_WORLD, &status);

    index=buffer;
    for (ix=0; ix<expr->ntx; ix++)
        for (iy=0; iy<expr->nty; iy++) {
	    for (iz=0; iz<expr->ntz; iz++)
	    	expr->sgn[0][ix][iy][iz] += *(index++);
	    for (iz=0; iz<expr->ntz; iz++)
	    	expr->sgn[1][ix][iy][iz] += *(index++);
	    for (iz=0; iz<expr->ntz; iz++)
	    	expr->sgn[2][ix][iy][iz] += *(index++);
    }
#ifdef MPI_DEBUG
    printf("[node %d]: mpi_recv_signal from %d : after recv() ", 
	myrank, status.MPI_SOURCE);
#endif

    run_nb++;
}

/*!
*************************************************************
* \ingroup MPI
* \brief Send the interesting part of the expr->magstate[] table
* to a given computation node.
*************************************************************/
static void
mpi_send_context(EXPERIENCE3D * expr, int x, int dst)
{
    int ix, iy;
    static int run_nb=0;
    static int buffer_size=0;
    static double *buffer;
    double *index;
    int slice_width;
#ifdef MPI_DEBUG
    int myrank;
#endif

#ifdef MPI_DEBUG
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
#endif

    if (run_nb==0) {
	buffer_size=expr->nfovz*3*expr->nfovy*expr->mpi_slice_width;
	buffer = (double *)malloc(buffer_size*sizeof(double));
    }

    slice_width = mpi_get_slice_width (expr, x);
    buffer_size=expr->nfovz*3*expr->nfovy*slice_width;

    index=buffer;
    for (ix=x; ix<x+slice_width; ix++)
	for (iy=0; iy<expr->nfovy; iy++) {
	    memcpy(index,expr->magstate[0][ix][iy],expr->nfovz*sizeof(double));
	    index += expr->nfovz;
	    memcpy(index,expr->magstate[1][ix][iy],expr->nfovz*sizeof(double));
	    index += expr->nfovz;
	    memcpy(index,expr->magstate[2][ix][iy],expr->nfovz*sizeof(double));
	    index += expr->nfovz;
	}

#ifdef MPI_DEBUG
    printf("[node %d]: mpi_send_context to node %d : ", myrank, dst);
    mpi_context_print_stats (expr,x);
#endif

    MPI_Send(buffer, buffer_size, MPI_DOUBLE, dst, WORKTAG, MPI_COMM_WORLD);
    run_nb++;
}

/*!
*************************************************************
* \ingroup MPI
* \brief Send the signal table to a given node.
*************************************************************/
static void
mpi_send_signal(EXPERIENCE3D * expr, int dst)
{
    int ix, iy;
    static int run_nb=0;
    static int buffer_size=0;
    static float *buffer;
    float *index;
#ifdef MPI_DEBUG
    int myrank;
#endif

#ifdef MPI_DEBUG
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
#endif

    if (run_nb==0) {
        buffer_size=3*expr->ntx*expr->nty*expr->ntz;
	buffer = (float *)malloc(buffer_size*sizeof(float));
    }

    index=buffer;
    for (ix=0; ix<expr->ntx; ix++)
	for (iy=0; iy<expr->nty; iy++) {
	    memcpy(index,expr->sgn[0][ix][iy],expr->ntz*sizeof(float));
	    index += expr->ntz;
	    memcpy(index,expr->sgn[1][ix][iy],expr->ntz*sizeof(float));
	    index += expr->ntz;
	    memcpy(index,expr->sgn[2][ix][iy],expr->ntz*sizeof(float));
	    index += expr->ntz;
	}
#ifdef MPI_DEBUG
    printf("[node %d]: mpi_send_signal to node %d : ", myrank, dst);
#endif
    MPI_Send(buffer, buffer_size, MPI_FLOAT, dst, WORKTAG, MPI_COMM_WORLD);
    run_nb++;
}

/*!
*************************************************************
* \ingroup MPI
* \brief code executed by the master node. The goal of this node is to
* assign dynamically a work slice to each node, and to automatically
* feed another work slice when a node completes is current task.  This
* scheme scales well in an heterogeneous network, because both fast and slow
* nodes work at their own speed, without slowing down the whole process.
* Communication occur when the master assigns a task to a node and
* collects results at the end of the task. So the longer the task execution
* time is, the more efficient the parallelization is.
*************************************************************/
void mpi_job_master (SEQPARAM *seqparam, EXPERIENCE3D * expr)
{
    MPI_Status status;
    int assigned_node_nb;
    int completed_node_nb;
    int node_nb;
    int max_node_nb;
    int assigned_task_nb;
    int completed_task_nb;
    int died_task_nb=0;
    int dummy;
    static short *task_table = NULL;
    int i;
    int value;
#ifdef MPI_DEBUG
    int myrank;
#endif

#ifdef MPI_DEBUG
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
#endif

    MPI_Comm_size(MPI_COMM_WORLD, &node_nb);

    if (task_table == NULL)
	task_table = (short *)calloc(expr->nfovx, sizeof(short));

    max_node_nb = node_nb-1;
    if (max_node_nb * expr->mpi_slice_width > expr->nfovx)
        max_node_nb = ceil((double)expr->nfovx / expr->mpi_slice_width);
#ifdef MPI_DEBUG
    printf("[node %d]: max_node_nb=%d\n",myrank, max_node_nb);
#endif

    /*
     * First, we assign a job to each slave node
     */
    for (assigned_task_nb = 0, assigned_node_nb = 1; 
	assigned_node_nb <= max_node_nb; 
	assigned_task_nb += expr->mpi_slice_width, assigned_node_nb++) {
#ifdef MPI_DEBUG
	printf("[node %d]: assign task %d to node %d\n", myrank,
	    assigned_task_nb, assigned_node_nb);
#endif
	MPI_Send(&assigned_task_nb, 1, MPI_INT, 
	    assigned_node_nb, WORKTAG, MPI_COMM_WORLD);
	/*
	 * send current expr->magstate table values.
	 */
	mpi_send_context (expr, assigned_task_nb, 
		assigned_node_nb);
#ifdef MPI_DEBUG
	printf ("[node %d]: sent current etat[] "
		"for task %d to node %d\n",
		myrank, assigned_task_nb, assigned_node_nb);
#endif
	task_table[assigned_task_nb]=assigned_node_nb;
    }
	
    for (;;) {
	MPI_Recv(&value, 1, MPI_INT, 
	    MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	/*
	 * the master can only receive WORKTAG messages from its slave.
	 */
	if (status.MPI_TAG == WORKTAG) {
	    /*
	     * receive the modified expr->magstate[] table
	     */
	    mpi_recv_context(expr, value, status.MPI_SOURCE, status);
#ifdef MPI_DEBUG
	    printf ("[node %d]: received current etat[]"
		"from node %d\n", myrank, status.MPI_SOURCE);
#endif
	    /*
	     * receive the signal contribution from this node
	     * and update the expr->sgn[] table. This table from the
	     * master node is modified by adding each contribution.
	     */
	    mpi_recv_and_update_signal (expr, status.MPI_SOURCE, 
	    	status);
	    completed_task_nb = value;
	    completed_node_nb = status.MPI_SOURCE;
#ifdef MPI_DEBUG
	    printf("[node %d]: node %d completed task %d\n",
		myrank, completed_node_nb, completed_task_nb);
#endif
	    /*
	     * assign another job to this slave node, if any available
	     */
	    if (assigned_task_nb < expr->nfovx) {
#ifdef MPI_DEBUG
		printf("[node %d]: assign task %d to node %d\n", 
		    myrank, assigned_task_nb, completed_node_nb);
#endif
		MPI_Send(&assigned_task_nb, 1, MPI_INT, completed_node_nb, 
		    WORKTAG, MPI_COMM_WORLD);
		/*
		 * send current expr->magstate table values
		 */
		mpi_send_context (expr, assigned_task_nb, 
			completed_node_nb);
#ifdef MPI_DEBUG
		printf ("[node %d]: sent current etat[] "
			"for task %d to node %d\n",
			myrank, assigned_task_nb, completed_node_nb);
#endif
		task_table[assigned_task_nb]=completed_node_nb;
		assigned_task_nb += expr->mpi_slice_width;
	    } else {
		/*
		 * else send a termination signal to this node
		 */
#ifdef MPI_DEBUG
		printf("[node %d]: asking node %d to give up\n", 
		    myrank, completed_node_nb);
#endif
		MPI_Send(&dummy, 1, MPI_INT, completed_node_nb, 
		    DIETAG, MPI_COMM_WORLD);
		died_task_nb++;
		if (died_task_nb == max_node_nb) {
		    if (died_task_nb < node_nb-1) {
			/* send a termination signal to other nodes not
			 * involved in the computation (case there are more
			 * nodes available than tasks to be done)
			 */
			for (i=died_task_nb+1; i<node_nb; i++) {
			    MPI_Send(&dummy, 1, MPI_INT, i, DIETAG, MPI_COMM_WORLD);
#ifdef MPI_DEBUG
			    printf("[node %d]: asking node %d to give up\n", 
				myrank, i);
#endif
			}

		    }
#ifdef MPI_DEBUG
		    printf("[node %d]: all slave nodes finished,quitting...\n", 
			myrank);
#endif
		    return;
		}
	    }
	} 
    }
}

/*!
*************************************************************
* \ingroup MPI
* \brief Code executed by the slave (aka working) nodes to perform
* the sequence computation on a slice provided by the slave node.
* Two MPI tags can be received by the slave node :
*   - DIETAG : no more work remains in the work queue, the
*              node can exit now.
*   - WORKTAG: the node is sent the slice it'll have to work on.
*************************************************************/

void mpi_job_slave (SEQPARAM *seqparam, EXPERIENCE3D *expr,
	task_cb task)
{
    MPI_Status status;
    int x0;
    int k, ix, iy, iz;
#ifdef MPI_DEBUG
    int myrank;
#endif

#ifdef MPI_DEBUG
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
#endif

    while (1)
    {
	MPI_Recv(&x0, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	/*
	 * the node receives a tag from the master, asking to quit now.
	 */
	if (status.MPI_TAG == DIETAG) {
#ifdef MPI_DEBUG
	    printf("[node %d]: quitting\n",myrank);
#endif
	    break;
	}
#ifdef MPI_DEBUG
	printf("[node %d]: received jobs x=%d\n",myrank, x0);
#endif
	/*
	 * the node receives work to do from the master.
	 */
	if (status.MPI_TAG == WORKTAG) {
	    /*
	     * Receive the current expr->magstate table.
	     */
	    mpi_recv_context (expr, x0, 0, status);
	    /*
	     * Reset expr->sgn for this new slice.
	     */
	    for (k=0; k<3; k++)
	        for (ix=0; ix<expr->ntx; ix++)
	            for (iy=0; iy<expr->nty; iy++)
	                for (iz=0; iz<expr->ntz; iz++)
			    expr->sgn[k][ix][iy][iz] = 0.0;
#ifdef MPI_DEBUG
	    printf ("[node %d]: received current etat[]"
		"from node %d\n", myrank, status.MPI_SOURCE);
#endif
	}

	expr->mpi_slice_x0 = x0;
	/*
	 * execute the sequence
	 */
	task(seqparam, expr);
	/* 
	 * tell the master that the work on this slice is completed
	 */
	MPI_Send(&x0, 1, MPI_INT, 0, WORKTAG, MPI_COMM_WORLD);
	/*
	 * Send back the modified expr->magstate table.
	 */
	mpi_send_context (expr, x0, 0);
#ifdef MPI_DEBUG
	printf ("[node %d]: sent current etat[] to node %d.\n",
	    myrank, 0);
#endif
	/*
	 * Send back the modified expr->sgn table.
	 */
	mpi_send_signal (expr, 0);
    }
}

/*!
*************************************************************
* \ingroup MPI
* \brief do the effective parallelization of a sequence given 
* in the task_cb parameter. The master/slave model is used, the
* master node (0) dispatches work to its slaves nodes (1..n-1),
* and collects the results. The level of parallelism can be 
* tweaked by modifying the SLICE_WIDTH constant. The delay for
* a working node to perform an autonomous task, without communicating 
* to its master node, is related to the SLICE_WIDTH value.
*************************************************************/
void mpi_job (SEQPARAM *seqparam, EXPERIENCE3D * expr,
	task_cb task)
{
	int myrank;

	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	expr->mpi_slice_width = SLICE_WIDTH;

	if (myrank == 0)
	    mpi_job_master(seqparam, expr);
	else
	    mpi_job_slave(seqparam, expr, task);
}

/*!
*************************************************************
* \ingroup MPI
* \brief report the effective width of a slice, according to 
* its position in the X axis. The goal of this function, is to
* deal with the real size of the last slice, which is smaller than
* other slices, when nfovx is not a multiple of slice_width.
*************************************************************/
int mpi_get_slice_width (EXPERIENCE3D *expr, int x)
{
    /*
     * we assume here that 0 <= x <= expr->nfovx-1
     */
    if (x + expr->mpi_slice_width > expr->nfovx)
	return expr->nfovx - x;
    else
	return expr->mpi_slice_width;
}

#else
void mpi_job (SEQPARAM *seqparam, EXPERIENCE3D * expr,task_cb task)
{
	task(seqparam, expr);
}
#endif
