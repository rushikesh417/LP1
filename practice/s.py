from xmlrpc.server import SimpleXMLRPCServer 

def getSquare (n):
    return n * n

def factorial(n):
   fact = 1
   if(n<0):
       return -1
   for i in range(1, n+1):
       fact = fact * i
   return fact

port = 8800
server = SimpleXMLRPCServer(("localhost" , port) , logRequests=False)
server.register_function(getSquare , 'square_rpc')
server.register_function(factorial , 'factorial')


try:
   print("server is runnig on port" ,port)
   server.serve_forever()
    
except:
   print("Exit")





