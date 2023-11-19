from xmlrpc.server import SimpleXMLRPCServer

def factorial(n):
   fact = 1
   if(n<0):
       return -1
   for i in range(1, n+1):
       fact = fact * i
   return fact

def is_prime(n):
    if n in [2, 3]:
        return "True"
    if (n == 1) or (n % 2 == 0):
        return "False"
    r = 3
    while r * r <= n:
        if n % r == 0:
            return "False"
        r += 2
    return "True"

def getSquare(a):
    b = a*a
    return b

def greet(txt):
    return "message received: "+ txt+"\n"+"Hii! Client.."


port = 8000
server = SimpleXMLRPCServer(("localhost", port), logRequests=False)
server.register_function(factorial, 'factorial_rpc')
server.register_function(getSquare, 'square_rpc')
server.register_function(greet,"greet_rpc")
server.register_function(is_prime,"is_prime_rpc")


try:
   print("Listening on the port ", port)
   server.serve_forever()

except:
   print("Exit")


