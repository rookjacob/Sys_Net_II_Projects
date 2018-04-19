/*
 * @file bbpeer.h
 *
 * @author Jacob Rook
 * @date 04/17/2018
 * @info Course COP 4635
 * Project 2
 */

#ifndef BBPEER_H_
#define BBPEER_H_

#include"headerFiles.h"

//Bulletin Board Message Formating Information
/*
 * These constant variables are used in the formating of the messages written to the shared file between the
 * peers. The messages written to the file are of fixed size (MESSAGE_SIZE). The format of the messages written
 * to the file follow the following format:
 *
 * <message n=number>
 * <message body>
 * </message>
 *
 */

const char HEADER[] = "<message n=%3d>\n";			//Format of the headers of messages posted to the bulletin board
const char FOOTER[] = "\n</message>\n";				//Format of the footers of messages posted to the bulletin board
#define MESSAGE_SIZE 256							//Size of the messages posted to the bulletin board
#define HEADER_SIZE 16								//Size of the bulletin board headers
#define FOOTER_SIZE 13								//Size of the bulletin board footers
#define BODY_SIZE (MESSAGE_SIZE - HEADER_SIZE - FOOTERSIZE)		//Size of the bulletin board body
//END Bulletin Board Message Formating Information




/*
 * 	@brief	message_Header_t	This structure is used to be able to notify the receiving peer what type of peer to peer
 * 	message is being sent. The valid values that the token and action variables can be set to are shown below with a
 * 	brief description.
 *
 * 	@var	token				Variable to indicate if the token is being passed, initialized, or not passed. Corresponding
 * 	token modes can be found below.
 *
 * 	@var	action				Variable to indicate the action that the message is requesting. Corresponding action modes
 * 	can be found below.
 */
struct message_Header_t{
	int token;
	int action;
};



//Peer Message Structure
/*
 * 	Contains the message header and the body of the message. The body of the message stores any data the peers
 * 	want to transfer from one peer to the next.
 */
/*
 * 	@brief	message_t			This structure is the structure that is passed amongst the peers. The peers send and receive
 * 	this structure to communicate with the other peers.
 *
 * 	@var	header				The header variable is used to identify or notify what the message that is being received or
 * 	sent is about.
 *
 * 	@var	messageBody			The messageBody variable is used to send data from one peer to the next.
 */
struct message_t{

	struct message_Header_t header;
	char messageBody[MESSAGE_SIZE + 1];

};



//Valid Peer Message Header Values
/*
 * 	The message header values are used to communicate among the peers in the ring. Communicate is based on passing of
 * 	the token and actions that other peers need to take. When a message is sent the token and action value of the header
 * 	is set as one of the token and action modes shown below. The description of the token and action modes can be found
 * 	below.
 *
 * 	There are a couple of rules for sending message among the peers, the rules are as follows:
 *
 * 	RULES:
 * 	Tokens can not be passed with actions.
 * 	Messages with invalid modes are discarded.
 */

//Token Modes
#define PASS_TOKEN 1024					//Indication that the token is being passed
#define NO_TOKEN 1025					//Indication that the token is not being passed
#define TOKEN_INIT 1026					//Indication that the initiation process is occurring

//Action Modes
#define NO_ACTION 2048					//Indication that there is no action to be taken
#define JOIN 2049						//Indication that a peer is wishing to join the ring
#define EXIT 2050						//Indication that a peer is wishing to exit the ring
//END Valid Peer Message Header Values



//Scope global variables used inside bulletinBoardRing()
/*
 * These variable are global to the bbpeer files to avoid passing many arguments between functions. Short descriptions
 * of what the variables are used for are given below.
 */
struct sockaddr_in NEXT_PEER_ADDR;				//Socket address of the next peer in the ring to send messages to
int NEXT_PEER_PORT;								//Integer port number of the next peer in the ring to send messages to
int HOST_PORT;									//Integer host port number
int SOCKET_D;									//Integer of the socket descriptor
char *FILENAME;									//Pointer to the filename argument passed by the user
struct message_t OUT_MESSAGE;					//Peer to peer message variable for sending messages to other peers
int HAVE_TOKEN;									//Variable to indicate that the peer has the token
//END Scope global variables used inside bulletinBoardRing()



//Variables for bulletinBoardEditing() thread

/*
 * These variables are used for the bulletinBoardEditing() thread to communicate with the main thread. When these
 * variables are not 0, the main thread knows that the user has input a command. The user can make four commands:
 * read, write, list, and exit.
 *
 * The bulletinBoardEditing() thread only allows the user to place one command at a time. The bulletinBoardEditing()
 * thread will allow the user to input another command when the main thread handles the last command and sets the
 * given command bit variable to 0.
 *
 * READ:
 * When the user wants to read from the bulletin board file, the bulletinBoardEditing() thread asks which message
 * the user would like to read and stores that message index into the READ_BIT variable. The next time the main thread
 * checks to see if the user has input a command, the main thread will see that the READ_BIT variable is non-zero and
 * will use the value of the READ_BIT to read the specified message.
 *
 * WRITE:
 * When the user wants to write to the bulletin board file, the bulletingBoardEditing() thread reads the message that
 * the user wants to write to the file and stores that message with the FOOTER string appended to the end of the message
 * in the WRITE_MESSAGE array, then sets the WRITE_BIT to 1. The next time the main thread checks to see if the user has
 * input a command, the main thread will see that the WRITE_BIT variable is non-zero and will handle the write command.
 *
 * LIST:
 * When the user wants to list all of the messages that have been written to the bulletin board file, the bulletinBoardEditing()
 * thread sets the LIST_BIT to a value of 1 to notify the main thread that the user wants to read all of the message from the
 * bulletin board file. When the main thread has printed out all of the messages that have been written to the bulletin board
 * file, the main thread will set the LIST_BIT to zero, to let the bulletinBoardEditing() thread can allow the user to make
 * another command.
 *
 * EXIT:
 * When the user wants to exit the ring, the bulletinBoardEditing thread sets the EXIT_BIT variable to a value of 1, then
 * terminates. When the main thread sees that the EXIT_BIT has been set to 1, the main thread handles the peer exiting the
 * ring. Once it is safe for the peer to exit, the main thread sets the EXIT_BIT equal to the value of the action mode
 * for EXIT to break it's self out of the continuous loop of handling messages.
 *
 */
int READ_BIT;									//Variable to indicate that the user wants to read one entry from the bulletin board file
int WRITE_BIT;									//Variable to indicate that the user wants to write to the bulletin board file
int LIST_BIT;									//Variable to indicate that the user wants to read all entries from the bulletin board file
int EXIT_BIT;									//Variable to indicate that the user wants to exit the ring
char WRITE_MESSAGE[MESSAGE_SIZE + 1];			//Array to store the message the user wants to write to the bulletin board file

//bulletinBoardEditing() thread control variables
pthread_t TID;									//Variable to store the thread ID of the bulletinBoardEditing() thread
pthread_mutex_t PRINT_LOCK;						//Mutex Lock used for printing to stdout
//END bulletinBoardEditing() thread control variables
//END Variables for bulletinBoardEditing() thread


/*
 * 	SLEEP_TIME is used to slow down the passing of the token to prevent the printing of the received token message from
 * 	crowding the stdout. When a peer receives the token and the user does not have a command, the peer sleeps for however
 * 	many seconds SLEEP_TIME is set to.
 */
#define SLEEP_TIME 5




//SUPPLEMENTARY FUNCTIONS
/*
 *	Generic functions that help multiple functions.
 */

/*
 *	@brief	error		The error() function produces an error message using perror() then terminates the program.
 *
 *	@param	msg			Message to display as the error message
 */
void error(const char *msg);

/*
 *	@brief	buildSocketAddress		The buildSocketAddress() function builds a socket address to the given port and
 *	stores the address to the location that is passed to the function. Since this implementation only handles peers
 *	that are on the same machine, the socket address is bound to the loopback IP address.
 *
 *	@param	socketAddress			This is the pointer to the location where the new socket address is to be stored.
 *
 *	@param	socketPort				This is the integer value of the port of the peer that the socket address is for.
 */
void buildSocketAddress(struct sockaddr_in *socketAddress, int socketPort);

/*
 *	@brief	initMessage				The initMessage() function initializes a peer to peer message structure with the
 *	given parameters.
 *
 *	@param	message					This points to the location of where the message is going to be stored.
 *
 *	@param	token					This is the value to store in the token variable of the message_Header_t structure
 *
 *	@param	action					This is the value to store in the action variable of the message_Header_t structure
 *
 *	@param	messageText				This is the string of data to store in the messageBody variable of the message_t structure
 */
void initMessage(struct message_t *message, int token, int action, char *messageText);

/*
 *	@brief	mutexPrint				The mutexPrint() function is used to prevent the print statements of the main and
 *	bulletinBoardEditing() thread from colliding when they want to print at the same time. The function will lock the
 *	print mutex lock then print the string that was passed to the function with a newline attached at the end, then
 *	release the lock.
 *
 *	@param	str						The string to be printed to stdout
 */
void mutexPrint(const char *str);

/*
 *	@brief	cleanStdin				The cleanStdin() function clears the stdin buffer. In the bulletinBoardEditing() thread,
 *	there was a problem of reading the users input because the buffer was not clear.
 */
void cleanStdin(void);
//END SUPLEMENTARY FUNCTIONS



//SET UP FUNCTIONS
/*
 * 	Functions that help start the ring.
 */

/*
 * @brief	validateArgv	The validateArgv() call checks if the arguments passed to the main function are in the correct
 * format and have appropriate values.
 *
 * @param	argc			Number of arguments passed to main()
 *
 * @param	argv			Argument vector passed to main()
 */
void validateArgv(int argc, char *argv[], int *sendPort);

/*
 *	@brief	getNextPeerFromServer	The getNextPeerFromServer() function takes the port number that is passed to the function
 *	and contacts the process with that port, assuming that the process is the server. The user is asked to enter a message to
 *	send to the server and sends the server the message. The function while then wait for the server to respond with the socket
 *	address of it's next peer in the ring. The function stores the socket address of the next peer to the NEXT_PEER_ADDR scope
 *	global variable and converts the socket address's port to an integer and stores it into the NEXT_PEER_PORT variable.
 *
 *	@param	serverPort				Integer value of the server's port number
 */
void getNextPeerFromServer(int serverPort);

/*
 *	@brief	getNextPeerFromPeer		The getNextPeerFromPeer() function takes the port number that is passed to the function and
 *	contacts the process with that port, assuming that the process is a peer in an already initialized ring. The function creates
 *	a message_t structure to send to the peer consisting of a string of the host's port number given by HOST_PORT to be stored in
 *	the messageBody and sets the action variable in the message_Header_t structure to indicate that the peer is wanting to join the
 *	peer. After the message is sent to the existing peer, the joining peer waits for a response with the address of it's next peer
 *	in the ring. When the address is received, it is stored in the socket address of the next peer to the NEXT_PEER_ADDR scope
 *	global variable and converts the socket address's port to an integer and stores it into the NEXT_PEER_PORT variable.
 *
 *	@param	peerPort				Integer value of the existing peer's port number
 */
void getNextPeerFromPeer(int peerPort);
//END SET UP FUNCTIONS



//BULLETIN BOARD RING FUNCTIONS
/*
 *	These functions are used to handle the operation of the ring structure between all of the peers on the main
 *	thread. At this	point the peers have been sent their next member in the ring, and are ready to start the ring.
 *
 */

/*
 *	@brief	bulletinBoardRing		The bulletinBoardRing() function handles the operations of the bulletin board ring
 *	structure. Whenever this function is called, it is assumed that all of the peers in the ring have their associated
 *	next peer in the ring to send messages that is stored in the NEXT_PEER_ADDR scope global variable. The function starts
 *	by determining the initiator of the ring, if the initiator has not already been found. If a peer is joining an existing
 *	ring, the peer should pass the function a value of 0, to indicate that it does not need to find an initiator. If the
 *	peer is apart of the ring that was just created, the peer needs to pass the function a 1, to indicate that the initiator
 *	needs to be found. Description on how the initiator is found can be found below in the Ring Initiation Functions section.
 *
 *	Once the initiator is determined, the bulletinBoardEditing thread is started and the ring starts the normal ring operations
 *	of passing messages between each other. The	normal ring operation is to received a message from a peer, process it, check to
 *	see if the current peer has the token, if so the current peer checks to see if the user has given any commands and handles
 *	the commands if there are commands. After the commands or no commands have been handled the current peer passes the token to
 *	the next peer in the ring. Then, waits for a new message to repeat the process. This process continues until the peer want to
 *	exit.
 *
 *	More details on how the ring operates can be found in the Peer to Peer Communication Handling Functions section and User Command
 *	Handling Functions section. More details on how the bulletinBoardEditing() thread works can be found in the bulletinBoardEditing()
 *	Thread Functions section.
 */
void bulletinBoardRing(int init);



//RING INITIATION FUNCTIONS
/*
 * 	@brief	setermineInitiator		The determineInitiator() function determines the initiator of the ring based on which peer has
 * 	the smallest port number. The peers send messages with the token variable set to TOKEN_INIT while the initiation process is
 * 	going. As long as the peer continues to receive messages with the token set to TOKEN_INIT the peer assumes that the initiation
 * 	process is still going.
 *
 * 	When a peer receives a message with the token variable set to TOKEN_INIT, the peer checks the messsageBody variable for the
 * 	value of the port number that was passed to the peer. If the port number that was passed to the peer is less than the host's port
 * 	number then the host knows that it is not the initiator because there is a peer with a port number less than its own. Therefore, the
 * 	peer forwards the message with the smaller port number to the next peer.
 *
 * 	If the host receives a message with a port number that is greater, then the host resends it's port number to the next peer in the
 * 	ring. If the host receives a message that has the same port number as, it knows that it must be the peer with the smallest port
 * 	number because all of the other peers must must have resent the host's port number because it was smaller than the rest.
 *
 * 	The initiator peer then sends a message to the next peer with the token variable set to NO_TOKEN to notify that peer that the
 * 	initiator has been determined. When the next peer receives the message with the token variable set to NO_TOKEN, that peer
 * 	will send a message to it's next peer in the ring to notify that peer that the initiator has been determined. This will continue
 * 	until all peers in the ring know that the initiator has been found.
 *
 * 	The initiator peer will then initiate the ring by calling the initRing() function which creates the bulletin board file and
 * 	sends out the first token to it's next peer in the ring. After this, the function ends and the normal operations of the
 * 	ring can begin.
 */
void determineInitiator(void);

/*
 *
 */
void initRing(void);
//END RING INITIATION FUNCTIONS



//PEER TO PEER COMMUNICATION HANDLING FUNCTIONS
/*
 *
 */
void processNextMessage(void);

/*
 *
 */
void handleJoin(struct sockaddr_in *joiningPeerAddr, struct message_t *receivedMessage);

/*
 *
 */
void handleExit(struct message_t *receivedMessage);
//END PEER TO PEER COMMUNICATION HANDLING FUNCTIONS



//USER COMMAND HANDLING FUNCTIONS
/*
 *
 */

/*
 *
 */
void checkUserInput(void);

/*
 *
 */
int getFileSize(void);

/*
 *
 */
void bulletinBoardWrite(void);

/*
 *
 */
void bulletinBoardRead(void);

/*
 *
 */
void bulletinBoardList(void);

/*
 *
 */
void bulletinBoardExit(void);
//END USER COMMAND HANDLING FUNCTIONS
//END BULLETIN BOARD RING FUNCTIONS



//BULLETINBOARDEDITING() THREAD FUNCTIONS
/*
 *	These functions are for the bulletinBoardEditing(), that handles the users input.
 */

/*
 *
 */
void *bulletinBoardEditing(void *parm);

/*
 *
 */
void userWrite(void);

/*
 *
 */
void userRead(void);

/*
 *
 */
void userList(void);

/*
 *
 */
void userExit(void);
//END BULLETINBOARDEDITING() THREAD FUNCTIONS






#endif /* BBPEER_H_ */
