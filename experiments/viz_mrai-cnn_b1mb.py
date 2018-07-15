''' Gather and visualise results from experiment '''
import numpy as np
import matplotlib.pyplot as plt

import _pickle as pickle


''' Experimental parameters '''

# Iterations
iI = range(0,50)
nI = len(iI)

# Number of labeled target patches
nT = np.array([1,2,5,10,20,50,100,200,500,1000,2000], dtype='int64')
lT = len(nT)

# Complexity of the classifier
cmplx = '_loglin'

''' Retrieve results '''

# Pre-allocate
err_scan = np.empty(( 3,lT,nI))
err_tiss = np.empty(( 6,lT,nI))

for n in range(lT):
	for i in range(nI):

		with open('results_cnn/err_exp_acqinv_cnn_b1mb_nT'+str(nT[n])+'_r'+str(iI[i]+1)+'.pickle', 'rb') as handle:
			e = pickle.load(handle, encoding='latin1')

		err_scan[ 0,n,i] = 2*(1 - 2*e['scan-H-source'])
		err_scan[ 1,n,i] = 2*(1 - 2*e['scan-H-net-un'])
		err_scan[ 2,n,i] = 2*(1 - 2*e['scan-H-net-tn'])
		err_tiss[ 0,n,i] = e['tiss-H-source']
		err_tiss[ 1,n,i] = e['tiss-H-srctgt']
		err_tiss[ 2,n,i] = e['tiss-H-net-un']
		err_tiss[ 3,n,i] = e['tiss-H-net-tn']
		err_tiss[ 4,n,i] = e['tiss-H-net-tt']
		err_tiss[ 5,n,i] = e['tiss-H-target']

scan_m = np.nanmean(err_scan,axis=2)
scan_s = np.nanstd(err_scan,axis=2) / np.sqrt(nI)
tiss_m = np.nanmean(err_tiss,axis=2)
tiss_s = np.nanstd(err_tiss,axis=2) / np.sqrt(nI)

'''	Visualize learning curves '''

# Visualisation parameters
fS = 20
lW = 10
plt.rc('text', usetex=True)

# Create figures
fig2, axs = plt.subplots(nrows=1, ncols=2, sharex=True, figsize=(20,5))
fig2.suptitle('Brainweb1.5T $->$ MRBrains',fontsize=fS, fontweight='heavy')

# Scanner error total
ax0 = axs[0]
ax0.plot(nT,scan_m[0,:], 'r', label='s+t')
ax0.fill_between(nT, scan_m[0,:] - scan_s[0,:], scan_m[0,:] + scan_s[0,:], alpha=0.2, edgecolor='r', facecolor='r',linewidth=lW, antialiased=True)
ax0.plot(nT,scan_m[2,:], 'b', label='rep')
ax0.fill_between(nT, scan_m[2,:] - scan_s[2,:], scan_m[2,:] + scan_s[2,:], alpha=0.2, edgecolor='b', facecolor='b',linewidth=lW, antialiased=True)

ax0.set_xscale('log')
ax0.set_ylim([0,2.02])
ax0.set_xlabel('number of target samples',fontsize=fS, fontweight='heavy')
ax0.set_ylabel(r'A-distance $d_{\cal A}(x,z)$',fontsize=fS, fontweight='heavy')
ax0.legend(fontsize=fS-2, loc=7, borderaxespad=0.1)

# Tissue error on total
ax2 = axs[1]
# ax2.plot(nT,tiss_m[0,:], 'r', label='src')
# ax2.fill_between(nT, tiss_m[0,:] - tiss_s[0,:], tiss_m[0,:] + tiss_s[0,:], alpha=0.2, edgecolor='r', facecolor='r',linewidth=lW, antialiased=True)
ax2.plot(nT,tiss_m[1,:], 'm', label='s+t-cnn')
ax2.fill_between(nT, tiss_m[1,:] - tiss_s[1,:], tiss_m[1,:] + tiss_s[1,:], alpha=0.2, edgecolor='m', facecolor='m',linewidth=lW, antialiased=True)
# ax2.plot(nT,tiss_m[3,:], 'b', label='net')
# ax2.fill_between(nT, tiss_m[3,:] - tiss_s[3,:], tiss_m[3,:] + tiss_s[3,:], alpha=0.2, edgecolor='b', facecolor='b',linewidth=lW, antialiased=True)
ax2.plot(nT,tiss_m[4,:], 'c', label='rep-svm')
ax2.fill_between(nT, tiss_m[4,:] - tiss_s[4,:], tiss_m[4,:] + tiss_s[4,:], alpha=0.2, edgecolor='c', facecolor='c',linewidth=lW, antialiased=True)
ax2.plot(nT,tiss_m[5,:], 'k', label='tgt-cnn')
ax2.fill_between(nT, tiss_m[5,:] - tiss_s[5,:], tiss_m[5,:] + tiss_s[5,:], alpha=0.2, edgecolor='k', facecolor='k',linewidth=lW, antialiased=True)

ax2.set_xscale('log')
ax2.set_ylim([0,1])
ax2.set_xlabel('number of target samples',fontsize=fS, fontweight='heavy')
ax2.set_ylabel('tissue classification error',fontsize=fS, fontweight='heavy')
ax2.legend(fontsize=fS-2, loc=0, borderaxespad=0.1)
# ax2.set_title('Tissues (val)',fontsize=fS, fontweight='bold')

[tick.label.set_fontsize(fS) for tick in ax0.xaxis.get_major_ticks()]
[tick.label.set_fontsize(fS) for tick in ax0.yaxis.get_major_ticks()]
[tick.label.set_fontsize(fS) for tick in ax2.xaxis.get_major_ticks()]
[tick.label.set_fontsize(fS) for tick in ax2.yaxis.get_major_ticks()]

# Write results
fig2.savefig('vis/err-lc_exp_acqinv_cnn_b1mb_H.png', bbox_inches='tight', pad_inches=0)

plt.show()
