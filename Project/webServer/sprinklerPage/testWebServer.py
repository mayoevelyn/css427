import pickle
import time
from threading import Thread

valveSeqNum = 0

while True:
    time.sleep(0.25)
    
    try:
        fp3 = open("formValveState.pkl")
        formValveState = pickle.load(fp3)
    
        if valveSeqNum <= formValveState["SeqNum"]:
            print valveSeqNum
            valveSeqNum = valveSeqNum + 1

        if formValveState["SeqNum"] == 0 and valveSeqNum == 10:
            valveSeqNum = 0           
            
    except:
        print "exception"
        
    
    

    
