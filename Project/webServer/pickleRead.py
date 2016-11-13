import pickle
fp = open ("shared.pkl")
shared = pickle.load(fp)
print shared["Foo"]
print shared["Honey"]
