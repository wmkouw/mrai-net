import scipy as sp
import numpy as np
import tensorflow as tf
import keras as ks
import sklearn as sk
import sklearn.model_selection as skms
import sklearn.linear_model as sklm
from keras import backend as bK

class acqinv_net:

    def __init__(self,m=1,w=256,h=256,K=np.array([1,2,3]),pD=3,nE=2,nR=1,sS=10,l2=0.001,nTl=1,cmplx='nonlin'):
        ''' Constructor'''
        
        self.m = m
        self.w = w
        self.h = h
        self.K = K
        self.pD = pD
        self.nE = nE
        self.nR = nR
        self.sS = sS
        self.l2 = l2
        self.nTl = nTl
        self.cmplx = cmplx

        # Patch width
        self.pW=2*pD+1

        # Patch unraveled width
        self.pU=(2*pD+1)**2

        # Initialize net architecture
        self.net = []


    def subject2image(self, fn):
        ''' Load subject images '''

        # Find number of subjects
        nS = len(fn)
        
        # Preallocate images
        I = np.empty((nS,self.w,self.h))

        # Loop over subjects
        for s in range(nS):
            
            # File identifier of current subjects filename
            with open(fn[s], 'rb') as fid:
                
                # Read file to list and normalize to [0,1]
                im = np.fromfile(fid, count=self.w*self.h, dtype='uint8') / 255.
                
                # Reshape to image, transpose and store
                I[s,:,:] = im.reshape((self.w,self.h)).T
        
        return I


    def restrict_classes(self,L):
		''' Map undesired classes to background '''

		# Number of subjects
		nS = L.shape[0]

		# Current labels
		lab = np.unique(L)

		# Loop over subjects
		for s in range(nS):

			# Map classes outside set of desired classes to background
			for k in np.setdiff1d(lab,self.K):
			    L[s][L[s]==k] = 0
		
		# New set of current labels	    
		lab = np.unique(L)

		return L,lab


    def strip_skull(self, I,L):
        ''' Strip the skull from an image '''
        
        # Number of subjects
        nS = I.shape[0]

        # Loop over subjects
        for s in range(nS):
                
            # Map backgroud pixels to 0
            I[s][L[s]==0] = 0

        return I


    def contrastive_loss(self, y_true, y_pred):
        ''' Contrastive loss from Hadsell-et-al.'06 '''

        return bK.sum(y_true * bK.abs(y_pred) + (1 - y_true) * bK.square(bK.maximum(self.m - y_pred, 0)))

    
    def norm_shape(shape1,shape2):
        ''' Find shape for norm '''

        return (shape1[0], 1)

        
    def l1_norm(self, vects):
        ''' Compute Manhattan distance '''

        return bK.sum(bK.abs(vects[0]-vects[1]), axis=1, keepdims=True)


    def l2_norm(self, vects):
        ''' Compute Euclidean distance '''

        return bK.sqrt(bK.maximum(bK.sum(bK.square(vects[0]-vects[1]), axis=1, keepdims=True), bK.epsilon()))


    def index_comb(self, arrays, out=None):
        ''' Generate combinations of two index arrays '''

        return np.array(np.meshgrid(arrays[0], arrays[1])).T.reshape(-1,2)

            
    def index2patch(self, I, index=np.empty((0,))):
        ''' Slice a patch from an image at a particular index '''

        # Number of patches to sample
        num_samples = index.shape[0]
        
        # Find 2D index of linear index
        w = np.empty((num_samples,), dtype='int64')
        h = np.empty((num_samples,), dtype='int64')
        if len(index.shape)==1:
            ix = np.empty((num_samples,2), dtype='int64')
            for n in range(num_samples):
                w[n],h[n] = np.unravel_index(index[n], (self.w,self.h))
        elif len(index.shape)==2:
            w = index[:,0]
            h = index[:,1]
        else:
            print('index has wrong shape')
        
        # Slice out patch(es)
        patches = np.empty((num_samples, self.pW, self.pW))
        for n in range(num_samples):
            patches[n,:,:] =  I[w[n]-self.pD:w[n]+self.pD+1, h[n]-self.pD:h[n]+self.pD+1]
            
        return patches.reshape((num_samples,self.pW,self.pW))


    def set_net(self, hidden_nodes=[64,16,2], act='relu', opt='rmsprop'):
        ''' Define network architecture '''
        
        # Setup input layers
        A = ks.layers.Input(shape=(self.pU+1,))
        B = ks.layers.Input(shape=(self.pU+1,))

        # Define a sequential model for a pipeline
        pipe = ks.models.Sequential()
        pipe.add(ks.layers.Dense(hidden_nodes[0], input_shape=(self.pU+1,), activation=act))
        for h in range(1,len(hidden_nodes)-1):
            pipe.add(ks.layers.Dropout(0.2))
            pipe.add(ks.layers.Dense(hidden_nodes[h], activation=act, kernel_regularizer=ks.regularizers.l2(self.l2)))
        pipe.add(ks.layers.Dense(hidden_nodes[-1]))

        print(pipe.summary())

        # Distance in embedding space
        distance = ks.layers.Lambda(self.l1_norm, output_shape=(1,))([pipe(A), pipe(B)])

        # Set model
        model = ks.models.Model(inputs=[A,B], outputs=distance)

        # Compile model with optimizer
        model.compile(loss=self.contrastive_loss, optimizer=opt)

        # Return compiled model
        self.net = model


    def train(self, I,L,J,yt):
        ''' Train the net '''

        # Number of subjects
        nI = I.shape[0]
        nJ = J.shape[0]

        # Number of classes
        nK = len(self.K)

        # Minimum of the number of target labels and sample subset
        minTS = min(self.nTl, self.sS)

        # Loop over source subjects
        for i in range(nI):
            print('At source subject '+str(i+1)+'/'+str(nI))
            
            for j in range(nJ):
                print('At target subject '+str(j+1)+'/'+str(nJ))

                # Pre-allocated data
                train_I = np.empty((2*(nK-1)**2*self.sS*minTS,self.pU+1))
                train_J = np.empty((2*(nK-1)**2*self.sS*minTS,self.pU+1))
                train_Y = np.empty((2*(nK-1)**2*self.sS*minTS,1))
                
                # Find random other source subject
                if nI==1:
                    o = i
                else:
                    o = np.random.choice(np.setdiff1d(np.arange(nI),i), size=1, replace=False)[0]

                # Loop over classes
                c = 0
                for k in range(1,nK):

                    # Subsample indices of tissues
                    ixk = np.random.choice(np.ravel_multi_index(np.argwhere(L[i]==self.K[k]).T, (self.w,self.h)), size=self.sS, replace=False)
                    ixo = np.random.choice(np.ravel_multi_index(np.argwhere(L[o]==self.K[k]).T, (self.w,self.h)), size=minTS, replace=False)
                    ixt = np.random.choice(yt[j,k,:].reshape((-1,)), size=minTS, replace=False)

                    # Generate pairwise index combinations
                    ixkk = self.index_comb((ixk,ixt))
                    ixko = self.index_comb((ixk,ixo))

                    # Sample patches sets
                    pIkk = self.index2patch(I[i], index=ixkk[:,0]).reshape(ixkk.shape[0],-1)
                    pJkk = self.index2patch(J[j], index=ixkk[:,1]).reshape(ixkk.shape[0],-1)
                    pIok = self.index2patch(I[o], index=ixko[:,1]).reshape(ixkk.shape[0],-1)
                    
                    # Augment patch with scanner ID
                    pIkk = np.append(pIkk,np.zeros((pIkk.shape[0],1)),axis=1)
                    pJkk = np.append(pJkk, np.ones((pJkk.shape[0],1)),axis=1)
                    pIok = np.append(pIok,np.zeros((pIok.shape[0],1)),axis=1)

                    # Store patches source-target kk
                    train_I[c*self.sS*minTS:(c+1)*self.sS*minTS,:] = pIkk
                    train_J[c*self.sS*minTS:(c+1)*self.sS*minTS,:] = pJkk
                    train_Y[c*self.sS*minTS:(c+1)*self.sS*minTS,:] = np.ones((self.sS*minTS,1))
                    c += 1
                    
                    # Store patches source-source kk
                    train_I[c*self.sS*minTS:(c+1)*self.sS*minTS,:] = pIkk
                    train_J[c*self.sS*minTS:(c+1)*self.sS*minTS,:] = pIok
                    train_Y[c*self.sS*minTS:(c+1)*self.sS*minTS,:] = np.ones((self.sS*minTS,1))
                    c += 1

                    for l in np.setdiff1d(range(1,nK),k):

                        # Subsample indices of other tissues
                        ixo = np.random.choice(np.ravel_multi_index(np.argwhere(L[o]==self.K[l]).T, (self.w,self.h)), size=minTS, replace=False)
                        ixt = np.random.choice(yt[j,l,:].reshape((-1,)), size=minTS, replace=False)
                        
                        # Generate pairwise index combinations
                        ixkl = self.index_comb((ixk,ixt))
                        ixol = self.index_comb((ixk,ixo))

                        # Sample patches sets
                        pIkl = self.index2patch(I[i], index=ixkl[:,0]).reshape(ixkl.shape[0],-1)
                        pJkl = self.index2patch(J[j], index=ixkl[:,1]).reshape(ixkl.shape[0],-1)
                        pIol = self.index2patch(I[o], index=ixol[:,1]).reshape(ixol.shape[0],-1)
                        
                        # Augment patch with scanner ID
                        pIkl = np.append(pIkl,np.zeros((pIkl.shape[0],1)),axis=1)
                        pJkl = np.append(pJkl, np.ones((pJkl.shape[0],1)),axis=1)
                        pIol = np.append(pIol,np.zeros((pIol.shape[0],1)),axis=1)

                        # Store sampled patches
                        train_I[c*self.sS*minTS:(c+1)*self.sS*minTS,:] = pIkl
                        train_J[c*self.sS*minTS:(c+1)*self.sS*minTS,:] = pJkl
                        train_Y[c*self.sS*minTS:(c+1)*self.sS*minTS,:] = np.zeros((self.sS*minTS,1))
                        c += 1
                        
                        # Store sampled patches
                        train_I[c*self.sS*minTS:(c+1)*self.sS*minTS,:] = pIkl
                        train_J[c*self.sS*minTS:(c+1)*self.sS*minTS,:] = pIol
                        train_Y[c*self.sS*minTS:(c+1)*self.sS*minTS,:] = np.zeros((self.sS*minTS,1))
                        c += 1

                # Report loss
                self.net.fit(x=[train_I,train_J], y=train_Y, epochs=self.nE, shuffle=True)


    def propagate(self, Ip):
        ''' Propagate a set of patches through acqinv-net '''
        
        # Define intermediate-output model
        inter_model = ks.models.Model(inputs=self.net.input, outputs=self.net.get_layer(index=2).get_output_at(1))

        # Feed forward
        return inter_model.predict([Ip, Ip])
    

    def classify(self, X,yX,Z,yZ, c_range=np.logspace(-5,2,8)):  
        ''' Train and test on data propagated through network '''

        # Propagate samples through network
        HX = self.propagate(X)
        HZ = self.propagate(Z)

        if self.cmplx=='lin':
            
            # Grid search
            if X.shape[0] <= len(self.K):
                bestC = 1
            else:
                lrgs = skms.GridSearchCV(estimator=sklm.LogisticRegression(), cv=5, param_grid=dict(C=c_range))
                lrgs.fit(HX,yX)
                bestC = lrgs.best_estimator_.C

            # Fit on all data with best regularization parameter
            lr = sklm.LogisticRegression(C=bestC)
            lr.fit(HX,yX)

            # Test (error)
            err = 1-lr.score(HZ,yZ)
            
        elif self.cmplx=='nonlin':
            
            # Grid search
            if X.shape[0] <= len(self.K):
                bestC = 1
            else:
                svgs = skms.GridSearchCV(estimator=sk.svm.SVC(), cv=5, param_grid=dict(C=c_range))
                svgs.fit(HX,yX)
                bestC = svgs.best_estimator_.C

            # Fit on all data with best regularization parameter
            sv = sk.svm.SVC(C=bestC)
            sv.fit(HX,yX)            

            # Test (error)
            err = 1-sv.score(HZ,yZ)

        return err

            
    def image2patch(self, I,L):
        ''' Sample set of patches from image '''

        # Shapes
        nI = I.shape[0]
        nK = len(self.K)

        # Preallocate input and output patches
        Ip = np.zeros((nI,nK-1,self.sS,self.pU+1))
        Il = np.zeros((nI,nK-1,self.sS), dtype=int)

        for i in range(nI):
            for k in range(nK-1):

                # Subsample indices
                ixk = np.random.choice(np.ravel_multi_index(np.argwhere(L[i]==self.K[k+1]).T, (self.w,self.h)), size=self.sS, replace=False)

                # Sample patch from class k
                pIk = self.index2patch(I[i], index=ixk).reshape((self.sS,-1))
                        
                # Augment patches with scanner ID and store
                Ip[i,k,:,:] = np.append(pIk,np.zeros((self.sS,1)),axis=1)
                
                # Store class labels
                Il[i,k,:] = np.ones((self.sS,))
        
        return Ip,Il

