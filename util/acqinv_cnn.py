import numpy as np
import nibabel as nib
import tensorflow as tf
import keras as ks
from keras.models import Model,Sequential
from keras.layers import Input,Conv2D,MaxPooling2D,Flatten,Dense,Lambda,Dropout,concatenate,Reshape
import sklearn as sk
import sklearn.model_selection as skms
import sklearn.linear_model as sklm
from keras import backend as bK
from scipy.ndimage.interpolation import zoom
import acqinv_util as aiu
import matplotlib.pyplot as plt

class acqinv_cnn:

    def __init__(self,m=1,w=256,h=256,K=np.array([1,2,3]),pD=3,nE=2,sS=10,bS=32,l2=0.001,nT=1):
        ''' Constructor'''
        
        self.m = m
        self.w = w
        self.h = h
        self.K = K
        self.pD = pD
        self.nE = nE
        self.bS = bS
        self.sS = sS
        self.l2 = l2
        self.nT = nT

        # Patch widths
        self.pW=2*pD+1
        self.pU=(2*pD+1)**2

        # Initialize net architecture
        self.net = []


    def strip_skull(self, I,M):
        ''' Strip the skull from image I based on a mask M '''
        
        # Number of subjects
        nS = I.shape[0]

        # Loop over subjects
        for s in range(nS):
                
            # Map backgroud pixels to 0
            I[s][M[s]] = 0

        return I


    def image2patch(self, I,L):
        ''' Sample set of patches from image '''

        # Shapes
        nI = I.shape[0]
        nK = len(self.K)

        # Preallocate input and output patches
        Ip = np.zeros((nI*nK*self.sS,self.pW,self.pW,1))
        Il = np.zeros((nI*nK*self.sS,), dtype='int64')

        c = 0
        for i in range(nI):
            for k in range(nK):

                # Find tissue in image
                Lik = np.argwhere(L[i][self.pD:-self.pD-1,self.pD:-self.pD-1]==self.K[k])+self.pD

                # Subsample indices
                ixk = np.random.choice(np.ravel_multi_index(Lik.T, (self.w,self.h)), size=self.sS, replace=False)

                # Sample patch from class k
                Ip[c*self.sS:(c+1)*self.sS,:,:,:] = self.index2patch(I[i], index=ixk)
                
                # Store class labels
                Il[c*self.sS:(c+1)*self.sS] = self.K[k]*np.ones((self.sS,))

                # Tick up
                c += 1
        
        return Ip,Il


    def image2allpatch(self, I, patch_dim=(3,3),stride=1):
        ''' Get all patches from image '''

        # Shapes
        W,H = I.shape

        # Patch deviations
        pW = (patch_dim[0]-1)/2
        pH = (patch_dim[1]-1)/2

        # Grid coordinates
        wc = range(pW,W-pW,stride)
        hc = range(pH,H-pH,stride)

        # Number of patches to extract
        nP = len(wc)*len(hc)

        # Generate grid
        tw,th = np.meshgrid(wc,hc)
        indices = np.concatenate((th.reshape((-1,1)),tw.reshape((-1,1))),axis=1)

        # Sample patches at grid
        Ip = self.index2patch(I, index=indices)

        return Ip


    def index2patch(self, I, index=np.empty((0,))):
        ''' Slice a patch from an image at a particular index '''

        # Number of patches to sample
        num_samples = index.shape[0]
        
        # Find 2D index of linear index
        w = np.empty((num_samples,), dtype='int64')
        h = np.empty((num_samples,), dtype='int64')
        if len(index.shape)==1:
            for n in range(num_samples):
                w[n],h[n] = np.unravel_index(index[n], (self.w,self.h))
        elif len(index.shape)==2:
            w = index[:,0]
            h = index[:,1]
        else:
            print('index has wrong shape')
        
        # Slice out patch(es)
        patches = np.empty((num_samples, self.pW, self.pW,1))
        for n in range(num_samples):
            patches[n,:,:,0] =  I[w[n]-self.pD:w[n]+self.pD+1, h[n]-self.pD:h[n]+self.pD+1]
            
        return patches


    def contrastive_loss(self, y_true, y_pred):
        ''' Contrastive loss from Hadsell-et-al.'06 '''

        return bK.sum(y_true * bK.square(y_pred) + (1 - y_true) * bK.abs(bK.maximum(self.m - y_pred, 0)))

    
    def norm_shape(shape1,shape2):
        ''' Find shape for norm '''

        return (shape1[0], 1)

        
    def l1_norm(self, vects):
        ''' Compute Manhattan distance '''

        return bK.sum(bK.abs(vects[0]-vects[1]), axis=1, keepdims=True)


    def index_comb(self, arrays, out=None):
        ''' Generate combinations of two index arrays '''

        return np.array(np.meshgrid(arrays[0], arrays[1])).T.reshape(-1,2)


    def set_net(self, opt='rmsprop'):
        ''' Define network architecture '''
        
        # Setup input layers
        A = Input(shape=(self.pW,self.pW,1))
        B = Input(shape=(self.pW,self.pW,1))
        idA = Input(shape=(1,))
        idB = Input(shape=(1,))

        # Convolutional pipeline
        pipeC = Sequential()
        pipeC.add(Conv2D(8, kernel_size=(3,3), activation='relu', padding='valid', input_shape=(self.pW,self.pW,1)))
        # pipeC.add(Conv2D(16, (5,5), activation='relu', padding='valid'))
        pipeC.add(MaxPooling2D(pool_size=(2,2), padding='valid'))
        pipeC.add(Dropout(0.2))
        pipeC.add(Flatten())

        pipeF = Sequential()
        pipeF.add(Reshape((self.pU,), input_shape=(self.pW,self.pW,1)))     

        # Dense pipeline
        pipeD = Sequential()
        # pipeD.add(Dense(16, activation='relu', input_shape=(pipeF.output_shape[1]+1,)))
        pipeD.add(Dense(16, activation='relu', input_shape=(pipeC.output_shape[1]+1,)))
        pipeD.add(Dropout(0.2))
        pipeD.add(Dense(8, activation='relu'))
        pipeD.add(Dense(2))

        # Distance in embedding space
        distance = Lambda(self.l1_norm, output_shape=(1,))([pipeD(concatenate([pipeC(A),idA])), pipeD(concatenate([pipeC(B),idB]))])

        # Set model
        model = Model(inputs=[A,B,idA,idB], outputs=distance)

        # Compile model with optimizer
        model.compile(loss=self.contrastive_loss, optimizer=opt)

        # Print model
        print(model.summary())
        
        # Return compiled model
        self.net = model


    def train(self, I,L,J,tix):
        ''' Train the net '''

        # Number of subjects
        nI = I.shape[0]
        nJ = J.shape[0]

        # Number of classes
        nK = len(self.K)

        # Minimum of the number of target labels and sample subset
        minTS = min(self.nT, self.sS)

        # Loop over source subjects
        for i in range(nI):
            print('At source subject '+str(i+1)+'/'+str(nI))
            
            for j in range(nJ):
                print('At target subject '+str(j+1)+'/'+str(nJ))

                # Pre-allocated data
                train_I = np.empty((2*nK**2*self.sS*minTS,self.pW,self.pW,1))
                train_J = np.empty((2*nK**2*self.sS*minTS,self.pW,self.pW,1))
                train_i = np.empty((2*nK**2*self.sS*minTS,1))
                train_j = np.empty((2*nK**2*self.sS*minTS,1))
                train_Y = np.empty((2*nK**2*self.sS*minTS,1))
                
                # Find random other source subject
                if nI==1:
                    o = i
                else:
                    o = np.random.choice(np.setdiff1d(np.arange(nI),i), size=1, replace=False)[0]

                # Loop over classes
                c = 0
                for k in range(nK):

                    # Find tissue in image
                    Lik = np.argwhere(L[i][self.pD:-self.pD-1,self.pD:-self.pD-1]==self.K[k])+self.pD
                    Lok = np.argwhere(L[o][self.pD:-self.pD-1,self.pD:-self.pD-1]==self.K[k])+self.pD

                    # Subsample indices of tissues
                    ixk = np.random.choice(np.ravel_multi_index(Lik.T, (self.w,self.h)), size=self.sS, replace=False)
                    ixo = np.random.choice(np.ravel_multi_index(Lok.T, (self.w,self.h)), size=minTS, replace=False)
                    ixt = np.random.choice(tix[j,k,:].reshape((-1,)), size=minTS, replace=False)

                    # Generate pairwise index combinations
                    ixkk = self.index_comb((ixk,ixt))
                    ixko = self.index_comb((ixk,ixo))

                    # Store patches source-target kk
                    train_I[c*self.sS*minTS:(c+1)*self.sS*minTS,:,:,:] = self.index2patch(I[i], index=ixkk[:,0])
                    train_J[c*self.sS*minTS:(c+1)*self.sS*minTS,:,:,:] = self.index2patch(J[j], index=ixkk[:,1])
                    train_i[c*self.sS*minTS:(c+1)*self.sS*minTS,:] = np.zeros((ixkk.shape[0],1))
                    train_j[c*self.sS*minTS:(c+1)*self.sS*minTS,:] =  np.ones((ixkk.shape[0],1))
                    train_Y[c*self.sS*minTS:(c+1)*self.sS*minTS,:] = np.ones((self.sS*minTS,1))
                    c += 1
                    
                    # Store patches source-source kk
                    train_I[c*self.sS*minTS:(c+1)*self.sS*minTS,:,:,:] = self.index2patch(I[i], index=ixkk[:,0])
                    train_J[c*self.sS*minTS:(c+1)*self.sS*minTS,:,:,:] = self.index2patch(I[o], index=ixko[:,1])
                    train_i[c*self.sS*minTS:(c+1)*self.sS*minTS,:] = np.zeros((ixkk.shape[0],1))
                    train_j[c*self.sS*minTS:(c+1)*self.sS*minTS,:] = np.zeros((ixko.shape[0],1))
                    train_Y[c*self.sS*minTS:(c+1)*self.sS*minTS,:] = np.ones((self.sS*minTS,1))
                    c += 1

                    for l in np.setdiff1d(np.arange(nK),k):

                        # Find tissue in image
                        Lol = np.argwhere(L[o][self.pD:-self.pD-1,self.pD:-self.pD-1]==self.K[l])+self.pD

                        # Subsample indices of other tissues
                        ixo = np.random.choice(np.ravel_multi_index(Lol.T, (self.w,self.h)), size=minTS, replace=False)
                        ixt = np.random.choice(tix[j,l,:].reshape((-1,)), size=minTS, replace=False)
                        
                        # Generate pairwise index combinations
                        ixkl = self.index_comb((ixk,ixt))
                        ixol = self.index_comb((ixk,ixo))

                        # Store sampled patches
                        train_I[c*self.sS*minTS:(c+1)*self.sS*minTS,:,:,:] = self.index2patch(I[i], index=ixkl[:,0])
                        train_J[c*self.sS*minTS:(c+1)*self.sS*minTS,:,:,:] = self.index2patch(J[j], index=ixkl[:,1])
                        train_i[c*self.sS*minTS:(c+1)*self.sS*minTS,:] = np.zeros((ixkl.shape[0],1))
                        train_j[c*self.sS*minTS:(c+1)*self.sS*minTS,:] =  np.ones((ixkl.shape[0],1))
                        train_Y[c*self.sS*minTS:(c+1)*self.sS*minTS,:] = np.zeros((self.sS*minTS,1))
                        c += 1
                        
                        # Store sampled patches
                        train_I[c*self.sS*minTS:(c+1)*self.sS*minTS,:,:,:] = self.index2patch(I[i], index=ixkl[:,0])
                        train_J[c*self.sS*minTS:(c+1)*self.sS*minTS,:,:,:] = self.index2patch(I[o], index=ixol[:,1])
                        train_i[c*self.sS*minTS:(c+1)*self.sS*minTS,:] = np.zeros((ixkl.shape[0],1))
                        train_j[c*self.sS*minTS:(c+1)*self.sS*minTS,:] = np.zeros((ixol.shape[0],1))
                        train_Y[c*self.sS*minTS:(c+1)*self.sS*minTS,:] = np.zeros((self.sS*minTS,1))
                        c += 1

                # Report loss
                self.net.fit(x=[train_I,train_J,train_i,train_j], y=train_Y, epochs=self.nE, batch_size=self.bS, shuffle=True, verbose=0)


    def propagate(self, Ip, scan_ID):
        ''' Propagate a set of patches through acqinv-net '''

        nP = Ip.shape[0]
        
        # Define intermediate-output model
        inter_model = Model(inputs=self.net.input, outputs=self.net.get_layer(index=7).get_output_at(1))

        # Feed forward
        return inter_model.predict([Ip, Ip, scan_ID*np.ones((nP,1)), scan_ID*np.ones((nP,1))])


    def segmentImage(self, model, I, clfr, scan_ID=1, patch_dim=(3,3)):
        ''' Take a new image and segment it '''

        # Shape of image
        W,H = I.shape

        # Compute minimum edges
        edgesW = (patch_dim[0]-1)/2
        edgesH = (patch_dim[1]-1)/2

        if clfr=='conv':
            # Take out patches
            Ip = self.image2allpatch(I, patch_dim=patch_dim, stride=1)

            # Classify embedded patches
            Pp = np.argmax(model.predict(Ip),axis=1)

            # Add minimum label back
            Pp += np.min(self.K)

        elif clfr=='netw':
            # Take out patches
            Ip = self.propagate(self.image2allpatch(I,patch_dim=patch_dim),scan_ID=scan_ID)

            # Classify embedded patches
            Pp = model.predict(Ip)

        elif clfr=='flat':
            # Take out patches
            Ip = self.image2allpatch(I,scan_ID=scan_ID,patch_dim=patch_dim)[:,:-1]
            
            # Classify embedded patches
            Pp = model.predict(Ip)

        else:
            print('Unknown model')

        # Reassemble prediction image
        Pr = np.reshape(Pp, [W-(patch_dim[0]-1), H-(patch_dim[1]-1)])

        # Return re-assembled prediction
        return Pr

