/*************************************************************************
* $Id: iddicom.h,v 1.1 2005/09/09 08:22:29 bellet Exp $
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
  
  Version 1.0  05/09/2005
*************************************************************************/

/*************************************************************************
*                                  * PROJET      : libido
* NOM DU MODULE : iddicom.h        * TYPE        : entete
* AUTEUR        : JPR              * CREATION    : 12/12/99
* VERSION       : 1.0              * REVISION    :  
* LANGAGE       : C                * 
*                                  *
**************************************************************************
*
*  Description : entete DICOM
*               
 
**************************************************************************
*
*  MODULES UTILISES :
*
**************************************************************************
*                     |              |
* REVISIONS :  DATE   | AUTEUR       |  DESCRIPTION
*---------------------|--------------|------------------------------------
*           	      | 	     | 	
	27=01-2000	JPR		ajout Elements LibIDO
**************************************************************************/
#include "idacr.h"

#ifdef _MSC_VER
	#if _MFC_VER>0x0700
		#include <winsock2.h>		// Pour ntohs - BigEndianeries -
	#else
		#include <winsock2.h>		// Pour ntohs - BigEndianeries -
	#endif
#else
	#include <netinet/in.h>  	// Pour ntohs - BigEndianeries -
#endif

typedef struct __DicomVr__{
	char * dicom_VR;
	char* dicom_VR_name;
} DICOM_VR;

extern DICOM_VR _ID_dicom_vr[];

//----------------

typedef struct __Dicom__el__{ 
	unsigned short int dicom_group; 
	unsigned short int dicom_elem; 
	char * dicom_type; 
	char* dicom_abr_group; 
	char* dicom_libelle ;
} DICOM_ELEMENTS;

extern DICOM_ELEMENTS _ID_dicom_elements[];

//----------------

typedef struct __dicom_info__{ 
	unsigned short int dicom_group; 
	unsigned short int dicom_elem; 
	char* dicom_info_libelle ;
	int dicom_info_ind;
} DICOM_INFO;

extern DICOM_INFO _ID_dicom_info[];


typedef struct __transfert_syntax__{ 
	char * transf_synt_libelle;
	char * transf_synt_code;
} TRANSFERT_SYNTAX;

extern TRANSFERT_SYNTAX _ID_transfert_syntax[];

//----------------

DICOM_ELEMENTS * Id_dicom_elements_get_entry(int);

//----------------

// Dicom Data Dictionary : Annex A Registery of DICOM unique identifiers (UID)
// tire de ctn/facilities/uid/dicom_uids.h

#define DICOM_SOPCLASSVERIFICATION		"1.2.840.10008.1.1"

#define DICOM_TRANSFERLITTLEENDIAN		"1.2.840.10008.1.2"
#define DICOM_TRANSFERLITTLEENDIANEXPLICIT	"1.2.840.10008.1.2.1"
#define DICOM_TRANSFERBIGENDIANEXPLICIT		"1.2.840.10008.1.2.2"
#define DICOM_TRANSFERJPEGBASELINEPROCESS1	"1.2.840.10008.1.2.4.50"
#define DICOM_TRANSFERJPEGEXTENDEDPROC2AND4	"1.2.840.10008.1.2.4.51"
#define DICOM_TRANSFERJPEGEXTENDEDPROC3AND5	"1.2.840.10008.1.2.4.52"
#define DICOM_TRANSFERJPEGSPECTRALPROC6AND8	"1.2.840.10008.1.2.4.53"
#define DICOM_TRANSFERJPEGSPECTRALPROC7AND9	"1.2.840.10008.1.2.4.54"
#define DICOM_TRANSFERJPEGFULLPROGRESSPROC10AND12 "1.2.840.10008.1.2.4.55"
#define DICOM_TRANSFERJPEGFULLPROGRESSPROC11AND13 "1.2.840.10008.1.2.4.56"
#define DICOM_TRANSFERJPEGLOSSLESSPROC14	"1.2.840.10008.1.2.4.57"
#define DICOM_TRANSFERJPEGLOSSLESSPROC15	"1.2.840.10008.1.2.4.58"
#define DICOM_TRANSFERJPEGEXTENDEDPROC16AND18	"1.2.840.10008.1.2.4.59"
#define DICOM_TRANSFERJPEGEXTENDEDPROC17AND19	"1.2.840.10008.1.2.4.60"
#define DICOM_TRANSFERJPEGSPECTRALPROC20AND22	"1.2.840.10008.1.2.4.61"
#define DICOM_TRANSFERJPEGSPECTRALPROC21AND23	"1.2.840.10008.1.2.4.62"
#define DICOM_TRANSFERJPEGFULLPROGRESSPROC24AND26 "1.2.840.10008.1.2.4.63"
#define DICOM_TRANSFERJPEGFULLPROGRESSPROC25AND27 "1.2.840.10008.1.2.4.64"
#define DICOM_TRANSFERJPEGLOSSLESSPROC28	"1.2.840.10008.1.2.4.65"
#define DICOM_TRANSFERJPEGLOSSLESSPROC29	"1.2.840.10008.1.2.4.66"
#define DICOM_TRANSFERJPEGLOSSLESSPROCFIRSTORDERREDICT "1.2.840.10008.1.2.4.70"


/* Define the UIDS for the service classes defined by the DICOM standard
*/
#define DICOM_SOPCLASSBASICSTUDYCONTENTNOTIFICATION "1.2.840.10008.1.9"
#define	DICOM_SOPCLASSSTORAGECOMMITMENTPUSHMODEL "1.2.840.10008.1.20.1"
#define	DICOM_WELLKNOWNSTORAGECOMMITMENTPUSHMODEL "1.2.840.10008.1.20.1.1"
#define	DICOM_SOPCLASSSTORAGECOMMITMENTPULLMODEL "1.2.840.10008.1.20.2"
#define	DICOM_WELLKNOWNSTORAGECOMMITMENTPULLMODEL "1.2.840.10008.1.20.2.1"

#define DICOM_STDAPPLICATIONCONTEXT		"1.2.840.10008.3.1.1.1"

#define	DICOM_SOPCLASSDETACHEDPATIENTMGMT	"1.2.840.10008.3.1.2.1.1"
#define DICOM_SOPCLASSDETACHEDPATIENTMGMTMETA	"1.2.840.10008.3.1.2.1.4"
#define DICOM_SOPCLASSDETACHEDVISITMGMT		"1.2.840.10008.3.1.2.2.1"
#define DICOM_SOPCLASSDETACHEDSTUDYMGMT		"1.2.840.10008.3.1.2.3.1"
#define DICOM_SOPCLASSSTUDYCOMPONENTMGMT	"1.2.840.10008.3.1.2.3.2"
#define DICOM_SOPCLASSMPPS			"1.2.840.10008.3.1.2.3.3"
#define DICOM_SOPCLASSDETACHEDRESULTSMGMT	"1.2.840.10008.3.1.2.5.1"
#define DICOM_SOPCLASSDETACHEDRESULTSMGMTMETA	"1.2.840.10008.3.1.2.5.4"
#define DICOM_SOPCLASSDETACHEDSTUDYMGMTMETA	"1.2.840.10008.3.1.2.5.5"
#define DICOM_SOPCLASSDETACHEDINTERPRETMGMT	"1.2.840.10008.3.1.2.6.1"

#define DICOM_SOPCLASSBASICFILMSESSION		"1.2.840.10008.5.1.1.1"
#define	DICOM_SOPCLASSBASICFILMBOX		"1.2.840.10008.5.1.1.2"
#define	DICOM_SOPCLASSBASICGREYSCALEIMAGEBOX	"1.2.840.10008.5.1.1.4"
#define	DICOM_SOPCLASSBASICCOLORIMAGEBOX	"1.2.840.10008.5.1.1.4.1"
#define	DICOM_SOPCLASSREFERENCEDIMAGEBOX	"1.2.840.10008.5.1.1.4.2"
#define	DICOM_SOPCLASSGREYSCALEPRINTMGMTMETA	"1.2.840.10008.5.1.1.9"
#define	DICOM_SOPCLASSREFGREYSCALEPRINTMGMTMETA	"1.2.840.10008.5.1.1.9.1"
#define	DICOM_SOPCLASSPRINTJOB			"1.2.840.10008.5.1.1.14"
#define	DICOM_SOPCLASSBASICANNOTATIONBOX	"1.2.840.10008.5.1.1.15"
#define	DICOM_SOPCLASSPRINTER			"1.2.840.10008.5.1.1.16"
#define	DICOM_SOPPRINTERINSTANCE		"1.2.840.10008.5.1.1.17"
#define	DICOM_SOPCLASSCOLORPRINTMGMTMETA	"1.2.840.10008.5.1.1.18"
#define	DICOM_SOPCLASSREFCOLORPRINTMGMTMETA	"1.2.840.10008.5.1.1.18.1"
#define DICOM_SOPCLASSVOILUT			"1.2.840.10008.5.1.1.22"
#define DICOM_SOPCLASSIMAGEOVERLAYBOX		"1.2.840.10008.5.1.1.24"

#define	DICOM_SOPCLASSSTOREDPRINT		"1.2.840.10008.5.1.1.27"
#define	DICOM_SOPCLASSHARDCOPYGRAYSCALEIMAGE	"1.2.840.10008.5.1.1.29"
#define	DICOM_SOPCLASSHARDCOPYCOLORIMAGE	"1.2.840.10008.5.1.1.30"

#define	DICOM_SOPCLASSCOMPUTEDRADIOGRAPHY	"1.2.840.10008.5.1.4.1.1.1"
#define DICOM_SOPCLASSDIGXRAYPRESENTATION       "1.2.840.10008.5.1.4.1.1.1.1"
#define DICOM_SOPCLASSDIGXRAYPROCESSING         "1.2.840.10008.5.1.4.1.1.1.1.1"
#define DICOM_SOPCLASSMAMMOXRPRESENTATION       "1.2.840.10008.5.1.4.1.1.1.2"
#define DICOM_SOPCLASSMAMMOXRPROCESSING         "1.2.840.10008.5.1.4.1.1.1.2.1"
#define DICOM_SOPCLASSINTRAORALPRESENTATION     "1.2.840.10008.5.1.4.1.1.1.3"
#define DICOM_SOPCLASSINTRAORALPROCESSING       "1.2.840.10008.5.1.4.1.1.1.3.1"
#define	DICOM_SOPCLASSCT			"1.2.840.10008.5.1.4.1.1.2"
#define	DICOM_SOPCLASSUSMULTIFRAMEIMAGE1993	"1.2.840.10008.5.1.4.1.1.3"
#define	DICOM_SOPCLASSUSMULTIFRAMEIMAGE		"1.2.840.10008.5.1.4.1.1.3.1"
#define	DICOM_SOPCLASSMR			"1.2.840.10008.5.1.4.1.1.4"
#define	DICOM_SOPCLASSNM1993			"1.2.840.10008.5.1.4.1.1.5"
#define	DICOM_SOPCLASSUS1993			"1.2.840.10008.5.1.4.1.1.6"
#define	DICOM_SOPCLASSUS			"1.2.840.10008.5.1.4.1.1.6.1"
#define	DICOM_SOPCLASSSECONDARYCAPTURE		"1.2.840.10008.5.1.4.1.1.7"
#define DICOM_SOPCLASSSTANDALONEOVERLAY		"1.2.840.10008.5.1.4.1.1.8"
#define DICOM_SOPCLASSSTANDALONECURVE		"1.2.840.10008.5.1.4.1.1.9"
#define	DICOM_SOPCLASSWAVEFORMSTORAGE		"1.2.840.10008.5.1.4.1.1.9.1"
#define	DICOM_SOPCLASSECGWAVEFORMSTORAGE	"1.2.840.10008.5.1.4.1.1.9.1.1"
#define DICOM_SOPCLASSSTANDALONEMODALITYLUT	"1.2.840.10008.5.1.4.1.1.10"
#define DICOM_SOPCLASSSTANDALONEVOILUT		"1.2.840.10008.5.1.4.1.1.11"
#define DICOM_SOPCLASSGREYSCALEPS		"1.2.840.10008.5.1.4.1.1.11.1"
#define	DICOM_SOPCLASSXRAYANGIO			"1.2.840.10008.5.1.4.1.1.12.1"
#define	DICOM_SOPCLASSXRAYFLUORO		"1.2.840.10008.5.1.4.1.1.12.2"
#define DICOM_SOPCLASSXRAYANGIOBIPLANE_RET      "1.2.840.10008.5.1.4.1.1.12.3"
#define	DICOM_SOPCLASSNM			"1.2.840.10008.5.1.4.1.1.20"
#define DICOM_SOPCLASSVLENDOSCOPIC              "1.2.840.10008.5.1.4.1.1.77.1.1"
#define DICOM_SOPCLASSVLMICROSCOPIC             "1.2.840.10008.5.1.4.1.1.77.1.2"
#define DICOM_SOPCLASSVLSLIDEMICROSCOPIC        "1.2.840.10008.5.1.4.1.1.77.1.3"
#define DICOM_SOPCLASSVLPHOTOGRAPHIC            "1.2.840.10008.5.1.4.1.1.77.1.4"
#define	DICOM_SOPCLASSBASICTEXTSR		"1.2.840.10008.5.1.4.1.1.88.11"
#define	DICOM_SOPCLASSENHANCEDSR		"1.2.840.10008.5.1.4.1.1.88.22"
#define	DICOM_SOPCLASSCOMPREHENSIVESR		"1.2.840.10008.5.1.4.1.1.88.33"
#define	DICOM_SOPCLASSKEYOBJECTNOTE		"1.2.840.10008.5.1.4.1.1.88.59"
#define	DICOM_SOPCLASSPET			"1.2.840.10008.5.1.4.1.1.128"
#define	DICOM_SOPCLASSSTANDALONEPETCURVE	"1.2.840.10008.5.1.4.1.1.129"
#define DICOM_SOPRTIMAGESTORAGE			"1.2.840.10008.5.1.4.1.1.481.1"
#define DICOM_SOPRTDOSESTORAGE			"1.2.840.10008.5.1.4.1.1.481.2"
#define DICOM_SOPRTSTRUCTURESETSTORAGE		"1.2.840.10008.5.1.4.1.1.481.3"
#define DICOM_SOPRTBREAMS                       "1.2.840.10008.5.1.4.1.1.481.4"
#define DICOM_SOPRTPLANSTORAGE			"1.2.840.10008.5.1.4.1.1.481.5"
#define DICOM_SOPRTBRACHYTREATMENT              "1.2.840.10008.5.1.4.1.1.481.6"
#define DICOM_SOPRTTREATMENTSUMMARY             "1.2.840.10008.5.1.4.1.1.481.7"

#define	DICOM_SOPPATIENTQUERY_FIND		"1.2.840.10008.5.1.4.1.2.1.1"
#define	DICOM_SOPPATIENTQUERY_MOVE		"1.2.840.10008.5.1.4.1.2.1.2"
#define	DICOM_SOPPATIENTQUERY_GET		"1.2.840.10008.5.1.4.1.2.1.3"

#define	DICOM_SOPSTUDYQUERY_FIND		"1.2.840.10008.5.1.4.1.2.2.1"
#define	DICOM_SOPSTUDYQUERY_MOVE		"1.2.840.10008.5.1.4.1.2.2.2"
#define	DICOM_SOPSTUDYQUERY_GET			"1.2.840.10008.5.1.4.1.2.2.3"

#define	DICOM_SOPPATIENTSTUDYQUERY_FIND		"1.2.840.10008.5.1.4.1.2.3.1"
#define	DICOM_SOPPATIENTSTUDYQUERY_MOVE		"1.2.840.10008.5.1.4.1.2.3.2"
#define	DICOM_SOPPATIENTSTUDYQUERY_GET		"1.2.840.10008.5.1.4.1.2.3.3"

#define	DICOM_SOPMODALITYWORKLIST_FIND		"1.2.840.10008.5.1.4.31"

