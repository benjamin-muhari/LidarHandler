#from LidarHandler import get_100_frames
#from LidarHandler import get_frame
#import time

#def PrintFrame_0(frame):
#    if len(frame) !=0:
#        print(frame[0])
#    else:
#        print("Null frame")

#def CompareLoadTimes():
#    start= time.time()
#    for i in range (10):
#        frame = get_frame(4)
#        PrintFrame_0(frame)

#    end= time.time()
#    oneBy1time = end -start;

#    start= time.time()
#    for frame in get_100_frames(4):
#        PrintFrame_0(frame)

#    end= time.time()

#    print("\n1by1, time elapsed:",oneBy1time)
#    print("\n100, time elapsed:", end-start)