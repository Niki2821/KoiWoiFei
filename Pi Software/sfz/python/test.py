import random
import time

kompass1 = 0
userinput = input("What: ")

while True:

    time.sleep(0.1)

    kompass1 = int(kompass1) + 1

    userinput = int(userinput)

    if int(kompass1) >=userinput-30 and int(kompass1) <=userinput :
        print("done")
        kompass1 = userinput

    elif int(kompass1) < 360:
        int(kompass1) - 360

    print(kompass1)

        


