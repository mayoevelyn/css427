def isHot(temperature):
    if temperature > 50:
        return True
    else:
        return False

def isHumid(humidity):
    if humidity > 50:
        return True
    else:
        return False

def isBright(brightness):
    if brightness > 50:
        return True
    else:
        return False

def isMoist(moisture):
    if moisture > 50:
        return True
    else:
        return False

def schedule(temperature, humidity, brightness, moisture):
    hot = isHot(temperature)
    humid = isHumid(humidity)
    bright = isBright(brightness)
    moist = isMoist(moisture)

    if  not humid and not moist:
        print("water: not humid and not moist")
        return

    if bright and not moist:
        print("water: sunny and not moist")
        return

    if hot and not moist:
        print("water: hot and not moist")
        return
    
    if hot and not humid and bright:
        print("water: hot and not humid and sunny")
        return

    print("don't water:\nhot: " + str(hot) + "\nhumid: " + str(humid) + "\nbright: " + str(bright) + "\nmoist: " + str(moist))
    
def main():
    schedule(99,99,99,99)

if __name__ == '__main__':
    main()
