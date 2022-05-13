# Module math
from collections import UserList
import math

#from matplotlib.style import use


userinput = input("Grad bitte: ")# grad
userinput = int(userinput)


#print (math.pi) 

ML = 300
MR = 60
MH = 180

xml = userinput + ML
xmr = userinput + MR
xmh = userinput + MH

xbmr = math.radians(xmr)
xbml = math.radians(xml)
xbmh = math.radians(xmh)

Rechts = math.sin(xbmr)
Links = math.sin(xbml)
Hinten = math.sin(xbmh)

round(Rechts, 3)
round(Links, 3)
round(Hinten, 3)



"""
if Rechts == -2.4492935982947064e-16
    Rechts = 0


elif Links == -2.4492935982947064e-16
    Links = 0

elif Hinten == -2.4492935982947064e-16
    Hinten = 0

"""

Rechts2 = round(Rechts, 2)
Links2 = round(Links, 2)
Hinten2 = round(Hinten, 2)




if  0.60<= Hinten2 <= 0.80:
   Hinten2 = 0.90

if  0.60<= Links2 <= 0.80:
   Links2 = 0.90

if  0.60<= Rechts2 <= 0.80:
    Rechts2 = 0.90




print(Rechts2)
print(Links2)
print(Hinten2)


"""

print("Sinus   ", userinput, "Grad:", math.sin(xbm))
print("Cosinus ", userinput, "Grad:", math.cos(xbm))
print("Tangens ", userinput, "Grad:", math.tan(xbm))

"""



