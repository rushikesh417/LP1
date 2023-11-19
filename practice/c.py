import xmlrpc.client
proxy = xmlrpc.client.ServerProxy('http://localhost:8800/')

while(True):
    n = int(input("Enter ur choice "))
    if (n == 1):
        k = int(input("Enter ur no "))
        print(str(proxy.square_rpc(k)))
    if (n == 2):
        k = int(input("Enter ur no "))
        print(str(proxy.factorial(k)))
    if (n == 3):
        break     
    if (n>2 or n<1):
        print ("Invalid input ")