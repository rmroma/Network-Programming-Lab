# C++ - Network Programming Lab - Messenger Project 

Hybrid P2P and clientserver messenger application that enable two clients to send and receive messages between one and the other.<p>
<b>The developed project will implement the following functionality:</b><p>

<dl>
  <dt>1. The project will include two apps: Messenger Server App (MSA) and Messenger Client App (MCA).</dt>
  <dt>2. The communication between the MCA and MSA will be TCP based (control messages).</dt>
  <dt>3. The communication between two MCA’s will be UDP based (interaction between two clients).</dt>
  <dt>4. To connect to the server the client will sign in using a username and a password (encryption is not required).</dt>
  <dd>- The client can create a new user with a new password (unless this user is already created).</dd>
  <dd>- To communicate with another client, the user will request toopen a session with the required user name.</dd>
  <dd>- The list of usernames and passwords will be saved into a file in the server side.</dd>
  <dt>5. Chat rooms (conference chat):</dt>
  <dd>- The client is able to open a chat room.</dd>
  <dd>- Each client can enter a chat room or leave it.</dd>
  <dd>- Once a client enters a chat room it will receive all messages sent by all the other clients in the room.</dd>
  <dd>- In order not to overload the server, all client communication is P2P using UDP and does not pass through the server.</dd>
  <dt>6. Messenger Server App CLI:</dt>
  <dd> lu -> list all users.</dd>
  <dd> lcu -> list all connected users.</dd>
  <dd> ls -> list all sessions (two clients communicating).</dd>
  <dd> lr -> list all rooms.</dd> 
  <dd> lru [room name] -> list all users in this room.</dd>
  <dd> x -> shutdown.</dd>
  <dt>7. Messenger Client App CLI:</dt>
  <dd> c [IP] ->  connect to the server in the given ip.</dd>
  <dd> lu -> print the user list from the server.</dd>
  <dd> lr -> print all rooms.</dd>
  <dd> lru [room name] ->  print all users in this roo.m</dd>
  <dd> login [user] [password] -> login with the user and password.</dd>
  <dd> register [user] [password] register the new user with the given password and login the user.</dd>
  <dd> cnr [room name] -> open a new chat room.</dd>
  <dd> or [room name] -> enter a chat room user can be connected to only to one other user or chat room at a time, calling o or or                                commands will disconnect other open session.</dd>
  <dd> s [message] -> send a message.</dd>
  <dd> l -> print the current status of the client (connected to “xyz”/not connected).</dd>
  <dd> cs -> disconnect the open session / exit from a room.</dd> 
  <dd> cr [room name] -> close a room.</dd>
  <dd> d -> disconnect from serve.r</dd>
  <dd> x -> close the app.</dd>
  <dt>8. The user will be notified in each change in the client state with a proper message printed to the screen.</dt>
  <dt>9. Each incoming message will be printed in the format: >[Moshe] What's up?</dt>
</dl>

video preview on youtube:  https://www.youtube.com/watch?v=bkCRAjCwcCo

![My image](http://imageshack.com/a/img921/738/REAyvj.jpg)


![My image](http://imageshack.com/a/img923/6629/nwJsY8.jpg)


![My image](http://imageshack.com/a/img924/2093/s5z6eS.jpg)


![My image](http://imageshack.com/a/img923/2961/yXl7TW.jpg)


![My image](http://imageshack.com/a/img923/1921/xGCh5x.jpg)
