rd /s/q D:\1122\Classify\test\lmdb\train
rd /s/q D:\1122\Classify\test\lmdb\val
convert_imageset.exe --shuffle --resize_height=0 --resize_width=0 D:\1122\Classify\test\temp\ D:\1122\Classify\test\temp\train.txt D:\1122\Classify\test\lmdb\train
convert_imageset.exe --shuffle --resize_height=0 --resize_width=0 D:\1122\Classify\test\temp\ D:\1122\Classify\test\temp\val.txt D:\1122\Classify\test\lmdb\val
compute_image_mean.exe D:\1122\Classify\test\lmdb\train D:\1122\Classify\test\caffemodel\mean.binaryproto
caffe.exe train -solver D:\1122\Classify\test\caffemodel\solver.prototxt
copy D:\1122\Classify\test\caffemodel\mean.binaryproto D:\1122\Classify\test
copy D:\1122\Classify\test\caffemodel\caffenet_train_iter_10000.caffemodel D:\1122\Classify\test\caffenet.caffemodel
del D:\1122\Classify\test\caffemodel\*.caffemodel
del D:\1122\Classify\test\caffemodel\*.solverstate
