#----------------------------------------------------------------------------
from LidarHandler import run_cpp_program

try:
    run_cpp_program()
except:
    print("c-api\n")
    run_cpp_program("dummy")
#----------------------------------------------------------------------------
#from LidarHandler import VeloParser
#import time

#vp = VeloParser()
#vp.open_live(4)

#start = time.time()
#for i in range(10):
#    print(vp.read_frame_vector()[0])
#end = time.time()
#autoconv_time = end - start

#start = time.time()
#for i in range(100):
#    print(len(vp.read_frame_pyobj()))
#end = time.time()

#print("Retreive auto converted list: ", autoconv_time)
#print("Retreive manualy built list:", end - start)