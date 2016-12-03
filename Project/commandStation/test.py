from datetime import datetime

def main():       
    print(datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
    hour = int(datetime.now().strftime("%H"))
    print str(hour + 3)
    if hour < 18:
        print "earlier"
    else:
        print "later"
    
        
    

    

if __name__ == '__main__':    
    main()
