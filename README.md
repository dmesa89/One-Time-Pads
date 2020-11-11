# One Time Pads
 Encrypts & Decrypts Messages

I createdfive small programs that encrypt and decrypt information using a one-time pad-like system. These programs combine multi-processing code with socket-based inter-process communication. It includes a compilation script.

enc_client
 This program connects to enc_server, and asks it to perform a one-time pad style encryption. By itself, enc_client doesn’t do the encryption - enc_server does.   The syntax of enc_client is as follows:

   $ enc_client plaintext key port

 In this syntax, plaintext is the name of a file in the current directory that contains the plaintext you wish to encrypt. Similarly, key contains the encryption 
 key you wish to use to encrypt the text. Finally, port is the port that enc_client should attempt to connect to enc_server on. When enc_client receives the 
 ciphertext back from enc_server, it should output it to stdout. Thus, enc_client can be launched in any of the following methods, and should send its output 
 appropriately:

  $ enc_client myplaintext mykey 57171
  
  $ enc_client myplaintext mykey 57171 > myciphertext

If enc_client receives key or plaintext files with any bad characters in them, or the key file is shorter than the plaintext, then it terminates, send appropriate error text to stderr, and sets the exit value to 1. 
If enc_client should nog be able to connect to dec_server, the programs reject each other. 

dec_server
 This program performs exactly like enc_server, in syntax and usage. In this case, however, dec_server will decrypt ciphertext it is given, using the passed-in 
 ciphertext and key. Thus, it returns plaintext again to dec_client.

dec_client
 Similarly, this program will connect to dec_server and will ask it to decrypt ciphertext using a passed-in ciphertext and key, and otherwise performs exactly like enc_client, and is runnable in the same ways. 

keygen
  This program creates a key file of specified length. The characters in the file generated will be any of the 27 allowed characters, generated using the  
  standard Unix randomization methods. 

 The syntax for keygen is as follows:

   $ keygen keylength
 
 where keylength is the length of the key file in characters. keygen outputs to stdout.

 Here is an example run, which creates a key of 256 characters and redirects stdout a file called mykey (note that mykey is 257 characters long because of the   
 newline):

  $ keygen 256 > mykey
