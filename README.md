# Old_FaceRecFW

This repo holds source code of an old face recognition program I implemented in Qt C++ back in 2012. Through time, I needed to re-configure it to make it running on newer versions of Qt and OpenCV, so I only store the source code here. It shall work as expected but need time to set it up with current QT.

## Algorithms work through

_Phase 1_: Adaboost for face detection, which is supported by OpenCV.
_Phase 2_: Pre-processing for detected face images, including: binaryzation, rotation, clipping (removing useless areas), and sampling binary values representing the image into a vector.
_Phase 3_: Gabor wavelet transformation upon the incoming vector, plus PCA and fisher-LDA algorithm to improve the facial feature acquired.
_Phase 4_: Store the incoming vector or compare its similarity with existing ones then human face is recognised. 
