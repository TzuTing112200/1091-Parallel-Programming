#
# Paste the data you wish to graph in tab-delimited rows in the format:
#
#       xdata <tab> ydata
#
# In this example the xdata is time (s) and y data is y position (cm)
#


data = """
         8          0.000001617
        16          0.000001127
        32          0.000001154
        64          0.000001174
       128          0.000001352
       256          0.000001463
       512          0.000002188
      1024          0.000002416
      2048          0.000002863
      4096          0.000009203
      8192          0.000011782
     16384          0.000015027
     32768          0.000016656
     65536          0.000021137
    131072          0.000029915
    262144          0.000042967
    524288          0.000062788
   1048576          0.000090084
   2097152          0.000155052
   4194304          0.000454582
   8388608          0.001214360
  16777216          0.002469908
  33554432          0.004980783
  67108864          0.009998656
 134217728          0.019955331
 268435456          0.040658302
 536870912          0.079423243
1073741824          0.158310624
""".split('\n')  # split this string on the "newline" character.

#print len(data)


#
# The data is stored in a single string. Now, split the data and store
# each column in a list. Convert the data from a string to a float.
#

tlist1 = []
ylist1 = []
for s in data:
    if s:
        t,y = s.split()     # split the string in two
        t=float(t)          # convert time to float
        y=float(y)    # convert y-position (cm) to float in meters
        tlist1.append(t)     # append to the list for time data
        ylist1.append(y)     # append to the list for y-position data
        
#print "tlist=",tlist
#print "ylist=",ylist
#


data = """
         8          0.000054688
        16          0.000054062
        32          0.000053465
        64          0.000053310
       128          0.000052945
       256          0.000052891
       512          0.000052857
      1024          0.000060972
      2048          0.000092797
      4096          0.000092507
      8192          0.000175580
     16384          0.000208127
     32768          0.000385416
     65536          0.000750950
    131072          0.001320046
    262144          0.002426977
    524288          0.004680433
   1048576          0.009172418
   2097152          0.018156839
   4194304          0.036329733
   8388608          0.072443891
  16777216          0.144204361
  33554432          0.287879695
  67108864          0.575128812
 134217728          1.150064689
 268435456          2.299164476
 536870912          4.597528071
1073741824          9.193829442
""".split('\n')  # split this string on the "newline" character.

#print len(data)


#
# The data is stored in a single string. Now, split the data and store
# each column in a list. Convert the data from a string to a float.
#

tlist2 = []
ylist2 = []
for s in data:
    if s:
        t,y = s.split()     # split the string in two
        t=float(t)          # convert time to float
        y=float(y)    # convert y-position (cm) to float in meters
        tlist2.append(t)     # append to the list for time data
        ylist2.append(y)     # append to the list for y-position data

import matplotlib.pyplot as plt
plt.title('y-position vs. time for falling cupcake paper')
plt.xlabel('Message Size (B)')
plt.ylabel('Time (s)')
plt.plot(tlist1,ylist1,'m.',color='red', label='case 1')
plt.plot(tlist2,ylist2,'m.',color='blue', label='case 2')
    
plt.legend()
plt.show()
