
import random




class us_werte:
    werte = [0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5]
    def lesen(self):

     for x in range(0,16):
         self.werte[x] = random.randint(0, 500)
     
     return self.werte
    


