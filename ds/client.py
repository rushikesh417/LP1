import xmlrpc.client
proxy = xmlrpc.client.ServerProxy('http://localhost:8000/')

while(True):
   c=int(input("1.GREET Message\n2.CHECK PRIME OR NOT\n3.SQUARE\n4.EXIT\n"))
   if(c==1):
       a=input("ENTER THE MESSAGE-")
       print(str(proxy.greet_rpc(a)))
   if(c==2):
       b=int(input("ENTER THE NUMBER :"))
       print(str(proxy.is_prime_rpc(b)))
   if(c==3):
       n=int(input("ENTER THE NUMBER : "))
       print(str(proxy.square_rpc(n)))
   if(c==4):
       break
   if(c>3 or c<0):
       print("ENTER CORRECT OPTION")