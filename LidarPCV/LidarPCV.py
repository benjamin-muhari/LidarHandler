#----------------------------------------------------------------------------
#-- C-Api -------------------------------------------------------------------
#----------------------------------------------------------------------------
#import loadCompare as lc

#lc.CompareLoadTimes()

#----------------------------------------------------------------------------
#from LidarHandler import run_cpp_program

#try:
#    run_cpp_program()
#except:
#    print("C-API\n")
#    run_cpp_program("dummy")
#----------------------------------------------------------------------------
from LidarHandler import VeloParser
import time

vp = VeloParser()
vp.open_live(4)

start = time.time()
for i in range(10):
    print(vp.read_frame_vector()[0])
end = time.time()
autoconv_time = end - start

print("-------------------------------------")

start = time.time()
for i in range(1000):
    print(vp.read_frame_pyobj()[0])
end = time.time()

print("Retreive auto converted list: ", autoconv_time)
print("Retreive manualy built list:", end - start)


#----------------------------------------------------------------------------
#-- Pybind ------------------------------------------------------------------
#----------------------------------------------------------------------------
#from LidarHandler import hw
#from LidarHandler import MyCord
##hw()
#a = MyCord()
#print(a.getX())
#a.setXY(2,3)
#print(a.getX())

#print(a.readIntQueue())

#for cord in a.readCordQueue():
#    print(cord)