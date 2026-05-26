#ifndef SDL3PP_NET_H_
#define SDL3PP_NET_H_

#include "SDL3pp_properties.h"
#include "SDL3pp_version.h"

#ifdef SDL3PP_ENABLE_NET

#include <SDL3_net/SDL_net.h>

namespace SDL {

/**
 * @defgroup CategorySDLNet Category SDLNet
 *
 * SDL_net is a simple library to help with networking.
 *
 * In current times, it's a relatively thin layer over system-level APIs like
 * BSD Sockets or WinSock. Its primary strength is in making those interfaces
 * less complicated to use, and handling several unexpected corner cases, so the
 * app doesn't have to.
 *
 * Some design philosophies of SDL_net:
 *
 * - Nothing is blocking (but you can explicitly wait on things if you want).
 * - Addressing is abstract so you don't have to worry about specific networks
 *   and their specific protocols.
 * - Simple is better than hard, and not necessarily less powerful either.
 *
 * There are several pieces to this library, and most apps won't use them all,
 * but rather choose the portion that's relevant to their needs.
 *
 * All apps will call NET.Init() on startup and NET.Quit() on shutdown.
 *
 * The cornerstone of the library is the Address object. This is what manages
 * the details of how to reach another computer on the network, and what network
 * protocol to use to get there. You'll need a Address to talk over the network.
 * If you need to convert a hostname (such as "google.com" or "libsdl.org") into
 * a Address, you can call ResolveHostname(), which will do the appropriate DNS
 * queries on a background thread. Once these are ready, you can use the Address
 * to connect to these hosts over the Internet.
 *
 * Something that initiates a connection to a remote system is called a
 * "client," connecting to a "server." To establish a connection, use the
 * Address you resolved with CreateClient(). Once the connection is established
 * (a non-blocking operation), you'll have a StreamSocket object that can send
 * and receive data over the connection, using WriteToStreamSocket() and
 * ReadFromStreamSocket().
 *
 * To instead be a server, that clients connect to, call CreateServer() to get a
 * Server object. All a Server does is allow you to accept connections from
 * clients, turning them into NET_StreamSockets, where you can read and write
 * from the opposite side of the connection from a given client.
 *
 * These things are, underneath this API, TCP connections, which means you can
 * use a client or server to talk to something that _isn't_ using SDL_net at
 * all.
 *
 * Clients and servers deal with "stream sockets," a reliable stream of bytes.
 * There are tradeoffs to using these, especially in poor network conditions.
 * Another option is to use "datagram sockets," which map to UDP packet
 * transmission. With datagrams, everyone involved can send small packets of
 * data that may arrive in any order, or not at all, but transmission can carry
 * on if a packet is lost, each packet is clearly separated from every other,
 * and communication can happen in a peer-to-peer model instead of
 * client-server: while datagrams can be more complex, these _are_ useful
 * properties not avaiable to stream sockets. CreateDatagramSocket() is used to
 * prepare for datagram communication, then SendDatagram() and ReceiveDatagram()
 * transmit packets.
 *
 * As previously mentioned, SDL_net's API is "non-blocking" (asynchronous). Any
 * network operation might take time, but SDL_net's APIs will not wait until
 * they complete. Any operation will return immediately, with options to check
 * if the operation has completed later. Generally this is what a video game
 * needs, but there are times where it makes sense to pause until an operation
 * completes; in a background thread this might make sense, as it could simplify
 * the code dramatically.
 *
 * The functions that block until an operation completes:
 *
 * - WaitUntilConnected
 * - WaitUntilInputAvailable
 * - WaitUntilResolved
 * - WaitUntilStreamSocketDrained
 *
 * All of these functions offer a timeout, which allow for a maximum wait time,
 * an immediate non-blocking query, or an infinite wait.
 *
 * Finally, SDL_net offers a way to simulate network problems, to test the
 * always-less-than-ideal conditions in the real world. One can programmatically
 * make the app behave like it's on a flakey wifi connection even if it's
 * running wired directly to a gigabit fiber line. The functions:
 *
 * - SimulateAddressResolutionLoss
 * - SimulateStreamPacketLoss
 * - SimulateDatagramPacketLoss
 *
 * @{
 */

// Forward decl
struct AddressBase;

// Forward decl
struct Address;

/// Alias to raw representation for Address.
using AddressRaw = NET_Address*;

/**
 * Reference for Address.
 *
 * This does not take ownership!
 */
using AddressRef = ResourceRefT<AddressBase>;

// Forward decl
struct StreamSocketBase;

// Forward decl
struct StreamSocket;

/// Alias to raw representation for StreamSocket.
using StreamSocketRaw = NET_StreamSocket*;

/**
 * Reference for StreamSocket.
 *
 * This does not take ownership!
 */
using StreamSocketRef = ResourceRefT<StreamSocketBase>;

// Forward decl
struct ServerBase;

// Forward decl
struct Server;

/// Alias to raw representation for Server.
using ServerRaw = NET_Server*;

/**
 * Reference for Server.
 *
 * This does not take ownership!
 */
using ServerRef = ResourceRefT<ServerBase>;

// Forward decl
struct DatagramSocketBase;

// Forward decl
struct DatagramSocket;

/// Alias to raw representation for DatagramSocket.
using DatagramSocketRaw = NET_DatagramSocket*;

/**
 * Reference for DatagramSocket.
 *
 * This does not take ownership!
 */
using DatagramSocketRef = ResourceRefT<DatagramSocketBase>;

// Forward decl
struct DatagramBase;

// Forward decl
struct Datagram;

/// Alias to raw representation for Datagram.
using DatagramRaw = NET_Datagram*;

/// Alias to const raw representation for Datagram.
using DatagramRawConst = const NET_Datagram*;

/**
 * Reference for Datagram.
 *
 * This does not take ownership!
 */
using DatagramRef = ResourceRefT<DatagramBase>;

/// Safely wrap Datagram for non owning const parameters
using DatagramConstRef = ResourceConstRef<DatagramRaw, DatagramRawConst>;

#ifdef SDL3PP_DOC

/**
 * The current major version of the SDL_net headers.
 *
 * If this were SDL_net version 3.2.1, this value would be 3.
 *
 * @since This macro is available since SDL_net 3.0.0.
 */
#define SDL_NET_MAJOR_VERSION

/**
 * The current minor version of the SDL_net headers.
 *
 * If this were SDL_net version 3.2.1, this value would be 2.
 *
 * @since This macro is available since SDL_net 3.0.0.
 */
#define SDL_NET_MINOR_VERSION

/**
 * The current micro (or patchlevel) version of the SDL_net headers.
 *
 * If this were SDL_net version 3.2.1, this value would be 1.
 *
 * @since This macro is available since SDL_net 3.0.0.
 */
#define SDL_NET_MICRO_VERSION

/**
 * This is the version number macro for the current SDL_net version.
 *
 * @since This macro is available since SDL_net 3.0.0.
 *
 * @sa NET.Version
 */
#define SDL_NET_VERSION                                                        \
  SDL_VERSIONNUM(                                                              \
    SDL_NET_MAJOR_VERSION, SDL_NET_MINOR_VERSION, SDL_NET_MICRO_VERSION)

/**
 * This macro will evaluate to true if compiled with SDL_net at least X.Y.Z.
 *
 * @since This macro is available since SDL_net 3.0.0.
 */
#define SDL_NET_VERSION_ATLEAST(X, Y, Z)                                       \
  ((SDL_NET_MAJOR_VERSION >= X) &&                                             \
   (SDL_NET_MAJOR_VERSION > X || SDL_NET_MINOR_VERSION >= Y) &&                \
   (SDL_NET_MAJOR_VERSION > X || SDL_NET_MINOR_VERSION > Y ||                  \
    SDL_NET_MICRO_VERSION >= Z))

#endif /* SDL3PP_DOC */

namespace NET {

/**
 * This function gets the version of the dynamically linked SDL_net library.
 *
 * @returns SDL_net version.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_net 3.0.0.
 */
inline int Version() { return NET_Version(); }

/**
 * Initialize the SDL_net library.
 *
 * This must be successfully called once before (almost) any other SDL_net
 * function can be used.
 *
 * It is safe to call this multiple times; the library will only initialize
 * once, and won't deinitialize until NET.Quit() has been called a matching
 * number of times. Extra attempts to init report success.
 *
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_net 3.0.0.
 *
 * @sa NET.Quit
 */
inline void Init() { CheckError(NET_Init()); }

/**
 * Deinitialize the SDL_net library.
 *
 * This must be called when done with the library, probably at the end of your
 * program.
 *
 * It is safe to call this multiple times; the library will only deinitialize
 * once, when this function is called the same number of times as NET.Init was
 * successfully called.
 *
 * Once you have successfully deinitialized the library, it is safe to call
 * NET.Init to reinitialize it for further use.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_net 3.0.0.
 *
 * @sa NET.Init
 */
inline void Quit() { NET_Quit(); }

} // namespace NET

/**
 * A tri-state for asynchronous operations.
 *
 * Lots of tasks in SDL_net are asynchronous, as they can't complete until data
 * passes over a network at some murky future point in time.
 *
 * This includes sending data over a stream socket, resolving a hostname,
 * connecting to a remote system, and other tasks.
 *
 * The library never blocks on tasks that take time to complete, with the
 * exception of functions named "Wait", which are intended to do nothing but
 * block until a task completes. Functions that are attempting to do something
 * that might block, or are querying the status of a task in-progress, will
 * return a Status, so an app can see if a task completed, and its final
 * outcome.
 *
 * @since This enum is available since SDL_net 3.0.0.
 */
using Status = NET_Status;

constexpr Status FAILURE =
  NET_FAILURE; ///< Async operation complete, result was failure.

/// Async operation is still in progress, check again later.
constexpr Status WAITING = NET_WAITING;

constexpr Status SUCCESS =
  NET_SUCCESS; ///< Async operation complete, result was success.

/**
 * Base class to Address.
 *
 * @see Address
 */
struct AddressBase : ResourceBaseT<AddressRaw>
{
  using ResourceBaseT::ResourceBaseT;

  /**
   * Drop a reference to an Address.
   *
   * Since several pieces of the library might share a single Address, including
   * a background thread that's working on resolving, these objects are
   * referenced counted. This allows everything that's using it to declare they
   * still want it, and drop their reference to the address when they are done
   * with it. The object's resources are freed when the last reference is
   * dropped.
   *
   * This function drops a reference to an Address, decreasing its reference
   * count by one.
   *
   * The documentation will tell you when the app has to explicitly unref an
   * address. For example, ResolveHostname() creates addresses that are already
   * referenced, so the caller needs to unref it when done.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_net 3.0.0.
   */
  void Unref();

  /**
   * Block until an address is resolved.
   *
   * The Address objects returned by ResolveHostname take time to do their work,
   * so it does so _asynchronously_ instead of making your program wait an
   * indefinite amount of time.
   *
   * However, if you want your program to sleep until the address resolution is
   * complete, you can call this function.
   *
   * This function takes a timeout value, represented in milliseconds, of how
   * long to wait for resolution to complete. Specifying a timeout of -1
   * instructs the library to wait indefinitely, and a timeout of 0 just checks
   * the current status and returns immediately (and is functionally equivalent
   * to calling GetAddressStatus).
   *
   * Resolution can fail after some time (DNS server took awhile to reply that
   * the hostname isn't recognized, etc), so be sure to check the result of this
   * function instead of assuming it worked!
   *
   * Once an address is successfully resolved, it can be used to connect to the
   * host represented by the address.
   *
   * If you don't want your program to block, you can call GetAddressStatus from
   * time to time until you get a non-zero result.
   *
   * @param timeout Number of milliseconds to wait for resolution to complete.
   *                -1 to wait indefinitely, 0 to check once without waiting.
   * @returns SUCCESS if successfully resolved, FAILURE if resolution failed,
   *          WAITING if still resolving (this function timed out without
   *          resolution); if FAILURE, call GetError() for details.
   *
   * @threadsafety It is safe to call this function from any thread, and several
   *               threads can block on the same address simultaneously.
   *
   * @since This function is available since SDL_net 3.0.0.
   *
   * @sa GetAddressStatus
   */
  Status WaitUntilResolved(Sint32 timeout);

  /**
   * Check if an address is resolved, without blocking.
   *
   * The Address objects returned by ResolveHostname take time to do their work,
   * so it does so _asynchronously_ instead of making your program wait an
   * indefinite amount of time.
   *
   * This function allows you to check the progress of that work without
   * blocking.
   *
   * Resolution can fail after some time (DNS server took awhile to reply that
   * the hostname isn't recognized, etc), so be sure to check the result of this
   * function instead of assuming it worked because it's non-zero!
   *
   * Once an address is successfully resolved, it can be used to connect to the
   * host represented by the address.
   *
   * @returns SUCCESS if successfully resolved, FAILURE if resolution failed,
   *          WAITING if still resolving (this function timed out without
   *          resolution); if FAILURE, call GetError() for details.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_net 3.0.0.
   *
   * @sa WaitUntilResolved
   */
  Status GetStatus();

  /**
   * Get a human-readable string from a resolved address.
   *
   * This returns a string that's "human-readable", in that it's probably a
   * string of numbers and symbols, like "159.203.69.7" or
   * "2604:a880:800:a1::71f:3001". It won't be the original hostname (like
   * "icculus.org"), but it's suitable for writing to a log file, etc.
   *
   * Do not free or modify the returned string; it belongs to the Address that
   * was queried, and is valid as long as the object lives. Either make sure the
   * address has a reference as long as you need this or make a copy of the
   * string.
   *
   * This will return nullptr if resolution is still in progress, or if
   * resolution failed. You can use GetAddressStatus() or WaitUntilResolved() to
   * make sure resolution has successfully completed before calling this.
   *
   * @returns a string on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_net 3.0.0.
   *
   * @sa GetAddressStatus
   * @sa WaitUntilResolved
   */
  const char* GetString();

  /**
   * Compare two Address objects.
   *
   * This compares two addresses, returning a value that is useful for qsort (or
   * qsort).
   *
   * @param b second address to compare.
   * @returns a value less than zero if `a` is "less than" `b`, a value greater
   *          than zero if "greater than", zero if equal.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_net 3.0.0.
   */
  int Compare(AddressRef b) const;

  /**
   * Compares two addresses for equality. Returns true if they are the same,
   * false otherwise.
   */
  bool operator==(const AddressBase& other) const;

  /**
   * Compares two addresses. Returns std::strong_ordering::less if this address
   * is less than the other, std::strong_ordering::greater if this address is
   * greater than the other, and std::strong_ordering::equal if they are equal.
   */
  auto operator<=>(const AddressBase& other) const;

  /**
   * Begin connecting a socket as a client to a remote server.
   *
   * Each StreamSocket represents a single connection between systems. Usually,
   * a client app will have one connection to a server app on a different
   * computer, and the server app might have many connections from different
   * clients. Each of these connections communicate over a separate stream
   * socket.
   *
   * Connecting is an asynchronous operation; this function does not block, and
   * will return before the connection is complete. One has to then use
   * WaitUntilConnected() or GetConnectionStatus() to see when the operation has
   * completed, and if it was successful.
   *
   * Once connected, you can read and write data to the returned socket. Stream
   * sockets are a mode of _reliable_ transmission, which means data will be
   * received as a stream of bytes in the order you sent it. If there are
   * problems in transmission, the system will deal with protocol negotiation
   * and retransmission as necessary, transparent to your app, but this means
   * until data is available in the order sent, the remote side will not get any
   * new data. This is the tradeoff vs datagram sockets, where data can arrive
   * in any order, or not arrive at all, without waiting, but the sender will
   * not know.
   *
   * Stream sockets don't employ any protocol (above the TCP level), so they can
   * connect to servers that aren't using SDL_net, but if you want to speak any
   * protocol beyond an abritrary stream of bytes, such as HTTP, you'll have to
   * implement that yourself on top of the stream socket.
   *
   * This function will fail if `address` is not finished resolving.
   *
   * When you are done with this connection (whether it failed to connect or
   * not), you must dispose of it with DestroyStreamSocket().
   *
   * Unlike BSD sockets or WinSock, you specify the port as a normal integer;
   * you do not have to byteswap it into "network order," as the library will
   * handle that for you.
   *
   * There are currently no extra properties for creating a client, so `props`
   * should be zero. A future revision of SDL_net may add additional (optional)
   * properties.
   *
   * @param port the port on the remote server to connect to.
   * @param props properties of the new client. Specify zero for defaults.
   * @returns  pending connection on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_net 3.0.0.
   *
   * @sa WaitUntilConnected
   * @sa GetConnectionStatus
   * @sa DestroyStreamSocket
   */
  StreamSocket CreateClient(Uint16 port, PropertiesRef props);

  /**
   * Create a server, which listens for connections to accept.
   *
   * An app that initiates connection to a remote computer is called a "client,"
   * and the thing the client connects to is called a "server."
   *
   * Servers listen for and accept connections from clients, which spawns a new
   * stream socket on the server's end, which it can then send/receive data on.
   *
   * Use this function to create a server that will accept connections from
   * other systems.
   *
   * This function does not block, and is not asynchronous, as the system can
   * decide immediately if it can create a server or not. If this returns
   * success, you can immediately start accepting connections.
   *
   * You can specify an address to listen for connections on; this address must
   * be local to the system, and probably one returned by GetLocalAddresses(),
   * but almost always you just want to specify nullptr here, to listen on any
   * address available to the app.
   *
   * After creating a server, you get stream sockets to talk to incoming client
   * connections by calling AcceptClient().
   *
   * Stream sockets don't employ any protocol (above the TCP level), so they can
   * accept connections from clients that aren't using SDL_net, but if you want
   * to speak any protocol beyond an abritrary stream of bytes, such as HTTP,
   * you'll have to implement that yourself on top of the stream socket.
   *
   * Unlike BSD sockets or WinSock, you specify the port as a normal integer;
   * you do not have to byteswap it into "network order," as the library will
   * handle that for you.
   *
   * The caller may supply properties to customize behavior. This is optional,
   * and a value of zero for `props` will request defaults for all properties.
   *
   * These are the supported properties:
   *
   * - `prop.Server.REUSEADDR_BOOLEAN`: true if the server should be created
   *   even if a previous server has recently used this address. For various
   *   reasons, networks prefer that there be some delay between apps reusing
   *   the same address, but this can be problematic when iterating quickly, for
   *   software development purposes or just restarting a crashed service. This
   *   property defaults to true (although it should be noted that, at the
   *   operating system level, this defaults to false!). If this property is
   *   false and the OS feels that not enough time has elapsed, server creation
   *   will fail and this function will report an error.
   *
   * @param port the port on the local address to listen for connections on.
   * @param props properties of the new server. Specify zero for defaults.
   * @returns a new Server on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_net 3.0.0.
   *
   * @sa GetLocalAddresses
   * @sa AcceptClient
   * @sa DestroyServer
   */
  Server CreateServer(Uint16 port, PropertiesRef props);

  /**
   * Create and bind a new datagram socket.
   *
   * Datagram sockets follow different rules than stream sockets. They are not a
   * reliable stream of bytes but rather packets, they are not limited to
   * talking to a single other remote system, they do not maintain a single
   * "connection" that can be dropped, and they are more nimble about network
   * failures at the expense of being more complex to use. What makes sense for
   * your app depends entirely on what your app is trying to accomplish.
   *
   * Generally the idea of a datagram socket is that you send data one chunk
   * ("packet") at a time to any address you want, and it arrives whenever it
   * gets there, even if later packets get there first, and maybe it doesn't get
   * there at all, and you don't know when anything of this happens by default.
   *
   * This function creates a new datagram socket.
   *
   * This function does not block, and is not asynchronous, as the system can
   * decide immediately if it can create a socket or not. If this returns
   * success, you can immediately start talking to the network.
   *
   * You can specify an address to listen for connections on; this address must
   * be local to the system, and probably one returned by GetLocalAddresses(),
   * but almost always you just want to specify nullptr here, to listen on any
   * address available to the app.
   *
   * If you need to bind to a specific port (like a server), you should specify
   * it in the `port` argument; datagram servers should do this, so they can be
   * reached at a well-known port. If you only plan to initiate communications
   * (like a client), you should specify 0 and let the system pick an unused
   * port. Only one process can bind to a specific port at a time, so if you
   * aren't acting as a server, you should choose 0. Datagram sockets can send
   * individual packets to any port, so this just declares where data will
   * arrive for your socket.
   *
   * Datagram sockets don't employ any protocol (above the UDP level), so they
   * can talk to apps that aren't using SDL_net, but if you want to speak any
   * protocol beyond arbitrary packets of bytes, such as WebRTC, you'll have to
   * implement that yourself on top of the stream socket.
   *
   * Unlike BSD sockets or WinSock, you specify the port as a normal integer;
   * you do not have to byteswap it into "network order," as the library will
   * handle that for you.
   *
   * The caller may supply properties to customize behavior. This is optional,
   * and a value of zero for `props` will request defaults for all properties.
   *
   * These are the supported properties:
   *
   * - `prop.DatagramSocket.REUSEADDR_BOOLEAN`: true if the socket should be
   *   created even if a previous socket has recently used this address. For
   *   various reasons, networks prefer that there be some delay between apps
   *   reusing the same address, but this can be problematic when iterating
   *   quickly, for software development purposes or just restarting a crashed
   *   service. This property defaults to true (although it should be noted
   *   that, at the operating system level, this defaults to false!). If this
   *   property is false and the OS feels that not enough time has elapsed,
   *   socket creation will fail and this function will report an error.
   * - `prop.DatagramSocket.ALLOW_BROADCAST_BOOLEAN`: true if the socket should
   *   allow broadcasting. At the lower level, this will set `SO_BROADCAST` for
   *   IPv4 sockets, to allow sending to the subnet's broadcast address at the
   *   OS level. For IPv6, it'll join the all-nodes link-local multicast group,
   *   ff02::1, allowing sending and receiving there, more or less simulating
   *   the usual IPv4 broadcast semantics. Other protocols take similar
   *   approaches. If you do not intend to send or receive broadcast packets on
   *   this socket, set this property to false, or omit it, as it defaults to
   *   false. Note: IPv4 will still be able to receive broadcast packets without
   *   this option, but IPv6 will not. Also see notes about sending to a
   *   broadcast address in SendDatagram().
   *
   * @param port the port on the local address to listen for connections on, or
   *             zero for the system to decide.
   * @param props properties of the new socket. Specify zero for defaults.
   * @returns a new DatagramSocket on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_net 3.0.0.
   *
   * @sa GetLocalAddresses
   * @sa DestroyDatagramSocket
   */
  DatagramSocket CreateDatagramSocket(Uint16 port, PropertiesRef props);
};

/**
 * Opaque representation of a computer-readable network address.
 *
 * This is an opaque datatype, to be treated by the app as a handle.
 *
 * SDL_net uses these to identify other servers; you use them to connect to a
 * remote machine, and you use them to find out who connected to you. They are
 * also used to decide what network interface to use when creating a server.
 *
 * These are intended to be protocol-independent; a given address might be for
 * IPv4, IPv6, or something more esoteric. SDL_net attempts to hide the
 * differences.
 *
 * @since This datatype is available since SDL_net 3.0.0.
 *
 * @sa ResolveHostname
 * @sa GetLocalAddresses
 * @sa CompareAddresses
 *
 * @cat resource
 */
struct Address : AddressBase
{
  using AddressBase::AddressBase;

  /**
   * Constructs from raw Address.
   *
   * @param resource a AddressRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit Address(AddressRaw resource) noexcept
    : AddressBase(resource)
  {
  }

  /// Move constructor
  constexpr Address(Address&& other) noexcept
    : Address(other.release())
  {
  }

  /**
   * Resolve a human-readable hostname.
   *
   * SDL_net doesn't operate on human-readable hostnames (like `www.libsdl.org`
   * but on computer-readable addresses. This function converts from one to the
   * other. This process is known as "resolving" an address.
   *
   * You can also use this to turn IP address strings (like "159.203.69.7") into
   * Address objects.
   *
   * Note that resolving an address is an asynchronous operation, since the
   * library will need to ask a server on the internet to get the information it
   * needs, and this can take time (and possibly fail later). This function will
   * not block. It either returns nullptr (catastrophic failure) or an
   * unresolved Address. Until the address resolves, it can't be used.
   *
   * If you want to block until the resolution is finished, you can call
   * WaitUntilResolved(). Otherwise, you can do a non-blocking check with
   * GetAddressStatus().
   *
   * When you are done with the returned Address, call UnrefAddress() to dispose
   * of it. You need to do this even if resolution later fails asynchronously.
   *
   * @param host The hostname to resolve.
   * @post A new Address on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_net 3.0.0.
   *
   * @sa WaitUntilResolved
   * @sa GetAddressStatus
   * @sa RefAddress
   * @sa UnrefAddress
   */
  Address(StringParam host);

  /**
   * Add a reference to an Address.
   *
   * Since several pieces of the library might share a single Address, including
   * a background thread that's working on resolving, these objects are
   * referenced counted. This allows everything that's using it to declare they
   * still want it, and drop their reference to the address when they are done
   * with it. The object's resources are freed when the last reference is
   * dropped.
   *
   * This function adds a reference to an Address, increasing its reference
   * count by one.
   *
   * The documentation will tell you when the app has to explicitly unref an
   * address. For example, ResolveHostname() creates addresses that are already
   * referenced, so the caller needs to unref it when done.
   *
   * Generally you only have to explicit ref an address when you have different
   * parts of your own app that will be sharing an address. In normal usage, you
   * only have to unref things you've created once (like you might free()
   * something), but you are free to add extra refs if it makes sense.
   *
   * This returns the same address passed as a parameter, which makes it easy to
   * ref and assign in one step:
   *
   * ```c
   * myAddr = RefAddress(yourAddr);
   * ```
   *
   * @param address The Address to add a reference to.
   * @post the same address that was passed as a parameter.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_net 3.0.0.
   */
  Address(const Address& address);

  /**
   * Safely borrows the from AddressRaw.
   *
   * @param resource a AddressRaw.
   *
   * This does not takes ownership!
   */
  static Address Borrow(AddressRaw resource)
  {
    if (resource) {
      NET_RefAddress(resource);
      return Address(resource);
    }
    return {};
  }

  /// Destructor
  ~Address() { NET_UnrefAddress(get()); }

  /// Assignment operator.
  constexpr Address& operator=(Address&& other) noexcept
  {
    swap(*this, other);
    return *this;
  }

  /// Assignment operator.
  Address& operator=(const Address& other)
  {
    if (get() != other.get()) {
      Address tmp(other);
      swap(*this, tmp);
    }
    return *this;
  }
};

/**
 * Resolve a human-readable hostname.
 *
 * SDL_net doesn't operate on human-readable hostnames (like `www.libsdl.org`
 * but on computer-readable addresses. This function converts from one to the
 * other. This process is known as "resolving" an address.
 *
 * You can also use this to turn IP address strings (like "159.203.69.7") into
 * Address objects.
 *
 * Note that resolving an address is an asynchronous operation, since the
 * library will need to ask a server on the internet to get the information it
 * needs, and this can take time (and possibly fail later). This function will
 * not block. It either returns nullptr (catastrophic failure) or an unresolved
 * Address. Until the address resolves, it can't be used.
 *
 * If you want to block until the resolution is finished, you can call
 * WaitUntilResolved(). Otherwise, you can do a non-blocking check with
 * GetAddressStatus().
 *
 * When you are done with the returned Address, call UnrefAddress() to dispose
 * of it. You need to do this even if resolution later fails asynchronously.
 *
 * @param host The hostname to resolve.
 * @returns A new Address on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_net 3.0.0.
 *
 * @sa WaitUntilResolved
 * @sa GetAddressStatus
 * @sa RefAddress
 * @sa UnrefAddress
 */
inline Address ResolveHostname(StringParam host)
{
  return Address(std::move(host));
}

inline Address::Address(StringParam host)
  : Address(CheckError(NET_ResolveHostname(host)))
{
}

inline Address::Address(const Address& address)
  : Address(address ? NET_RefAddress(address.get()) : nullptr)
{
}

/**
 * Block until an address is resolved.
 *
 * The Address objects returned by ResolveHostname take time to do their work,
 * so it does so _asynchronously_ instead of making your program wait an
 * indefinite amount of time.
 *
 * However, if you want your program to sleep until the address resolution is
 * complete, you can call this function.
 *
 * This function takes a timeout value, represented in milliseconds, of how long
 * to wait for resolution to complete. Specifying a timeout of -1 instructs the
 * library to wait indefinitely, and a timeout of 0 just checks the current
 * status and returns immediately (and is functionally equivalent to calling
 * GetAddressStatus).
 *
 * Resolution can fail after some time (DNS server took awhile to reply that the
 * hostname isn't recognized, etc), so be sure to check the result of this
 * function instead of assuming it worked!
 *
 * Once an address is successfully resolved, it can be used to connect to the
 * host represented by the address.
 *
 * If you don't want your program to block, you can call GetAddressStatus from
 * time to time until you get a non-zero result.
 *
 * @param address The Address object to wait on.
 * @param timeout Number of milliseconds to wait for resolution to complete. -1
 *                to wait indefinitely, 0 to check once without waiting.
 * @returns SUCCESS if successfully resolved, FAILURE if resolution failed,
 *          WAITING if still resolving (this function timed out without
 *          resolution); if FAILURE, call GetError() for details.
 *
 * @threadsafety It is safe to call this function from any thread, and several
 *               threads can block on the same address simultaneously.
 *
 * @since This function is available since SDL_net 3.0.0.
 *
 * @sa GetAddressStatus
 */
inline Status WaitUntilResolved(AddressRef address, Sint32 timeout)
{
  return NET_WaitUntilResolved(address, timeout);
}

inline Status AddressBase::WaitUntilResolved(Sint32 timeout)
{
  return SDL::WaitUntilResolved(get(), timeout);
}

/**
 * Check if an address is resolved, without blocking.
 *
 * The Address objects returned by ResolveHostname take time to do their work,
 * so it does so _asynchronously_ instead of making your program wait an
 * indefinite amount of time.
 *
 * This function allows you to check the progress of that work without blocking.
 *
 * Resolution can fail after some time (DNS server took awhile to reply that the
 * hostname isn't recognized, etc), so be sure to check the result of this
 * function instead of assuming it worked because it's non-zero!
 *
 * Once an address is successfully resolved, it can be used to connect to the
 * host represented by the address.
 *
 * @param address The Address to query.
 * @returns SUCCESS if successfully resolved, FAILURE if resolution failed,
 *          WAITING if still resolving (this function timed out without
 *          resolution); if FAILURE, call GetError() for details.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_net 3.0.0.
 *
 * @sa WaitUntilResolved
 */
inline Status GetAddressStatus(AddressRef address)
{
  return NET_GetAddressStatus(address);
}

inline Status AddressBase::GetStatus() { return SDL::GetAddressStatus(get()); }

/**
 * Get a human-readable string from a resolved address.
 *
 * This returns a string that's "human-readable", in that it's probably a string
 * of numbers and symbols, like "159.203.69.7" or "2604:a880:800:a1::71f:3001".
 * It won't be the original hostname (like "icculus.org"), but it's suitable for
 * writing to a log file, etc.
 *
 * Do not free or modify the returned string; it belongs to the Address that was
 * queried, and is valid as long as the object lives. Either make sure the
 * address has a reference as long as you need this or make a copy of the
 * string.
 *
 * This will return nullptr if resolution is still in progress, or if resolution
 * failed. You can use GetAddressStatus() or WaitUntilResolved() to make sure
 * resolution has successfully completed before calling this.
 *
 * @param address The Address to query.
 * @returns a string on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_net 3.0.0.
 *
 * @sa GetAddressStatus
 * @sa WaitUntilResolved
 */
inline const char* GetAddressString(AddressRef address)
{
  return CheckError(NET_GetAddressString(address));
}

inline const char* AddressBase::GetString()
{
  return SDL::GetAddressString(get());
}

/**
 * Add a reference to an Address.
 *
 * Since several pieces of the library might share a single Address, including a
 * background thread that's working on resolving, these objects are referenced
 * counted. This allows everything that's using it to declare they still want
 * it, and drop their reference to the address when they are done with it. The
 * object's resources are freed when the last reference is dropped.
 *
 * This function adds a reference to an Address, increasing its reference count
 * by one.
 *
 * The documentation will tell you when the app has to explicitly unref an
 * address. For example, ResolveHostname() creates addresses that are already
 * referenced, so the caller needs to unref it when done.
 *
 * Generally you only have to explicit ref an address when you have different
 * parts of your own app that will be sharing an address. In normal usage, you
 * only have to unref things you've created once (like you might free()
 * something), but you are free to add extra refs if it makes sense.
 *
 * This returns the same address passed as a parameter, which makes it easy to
 * ref and assign in one step:
 *
 * ```c
 * myAddr = RefAddress(yourAddr);
 * ```
 *
 * @param address The Address to add a reference to.
 * @returns the same address that was passed as a parameter.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_net 3.0.0.
 */
inline Address RefAddress(AddressRef address) { return Address(address); }

/**
 * Drop a reference to an Address.
 *
 * Since several pieces of the library might share a single Address, including a
 * background thread that's working on resolving, these objects are referenced
 * counted. This allows everything that's using it to declare they still want
 * it, and drop their reference to the address when they are done with it. The
 * object's resources are freed when the last reference is dropped.
 *
 * This function drops a reference to an Address, decreasing its reference count
 * by one.
 *
 * The documentation will tell you when the app has to explicitly unref an
 * address. For example, ResolveHostname() creates addresses that are already
 * referenced, so the caller needs to unref it when done.
 *
 * @param address The Address to drop a reference to.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_net 3.0.0.
 */
inline void UnrefAddress(AddressRaw address) { NET_UnrefAddress(address); }

inline void AddressBase::Unref() { UnrefAddress(release()); }

/**
 * Enable simulated address resolution failures.
 *
 * Often times, testing a networked app on your development machine--which might
 * have a wired connection to a fast, reliable network service--won't expose
 * bugs that happen when networks intermittently fail in the real world, when
 * the wifi is flakey and firewalls get in the way.
 *
 * This function allows you to tell the library to pretend that some percentage
 * of address resolutions will fail.
 *
 * The higher the percentage, the more resolutions will fail and/or take longer
 * for resolution to complete.
 *
 * Setting this to zero (the default) will disable the simulation. Setting to
 * 100 means _everything_ fails unconditionally. At what percent the system
 * merely borders on unusable is left as an exercise to the app developer.
 *
 * This is intended for debugging purposes, to simulate real-world conditions
 * that are various degrees of terrible. You probably should _not_ call this in
 * production code, where you'll likely see real failures anyhow.
 *
 * @param percent_loss A number between 0 and 100. Higher means more failures.
 *                     Zero to disable.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_net 3.0.0.
 */
inline void SimulateAddressResolutionLoss(int percent_loss)
{
  NET_SimulateAddressResolutionLoss(percent_loss);
}

/**
 * Compare two Address objects.
 *
 * This compares two addresses, returning a value that is useful for qsort (or
 * qsort).
 *
 * @param a first address to compare.
 * @param b second address to compare.
 * @returns a value less than zero if `a` is "less than" `b`, a value greater
 *          than zero if "greater than", zero if equal.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_net 3.0.0.
 */
inline int CompareAddresses(AddressRef a, AddressRef b)
{
  return NET_CompareAddresses(a, b);
}

inline int AddressBase::Compare(AddressRef b) const
{
  return SDL::CompareAddresses(get(), b);
}

inline bool AddressBase::operator==(const AddressBase& other) const
{
  return Compare(other) == 0;
}

inline auto AddressBase::operator<=>(const AddressBase& other) const
{
  return Compare(other) <=> 0;
}

/// @private
struct LocalAddressesArrayDeleter
{
  static void operator()(AddressRef* addresses);
};

/**
 * Array of addresses returned by GetLocalAddresses. The array is freed
 * automatically when it goes out of scope.
 */
using LocalAddressesArray = OwnArray<AddressRef, LocalAddressesArrayDeleter>;

/**
 * Obtain a list of local addresses on the system.
 *
 * This returns addresses that you can theoretically bind a socket to, to accept
 * connections from other machines at that address.
 *
 * You almost never need this function; first, it's hard to tell _what_ is a
 * good address to bind to, without asking the user (who will likely find it
 * equally hard to decide). Second, most machines will have lots of _private_
 * addresses that are accessible on the same LAN, but not public ones that are
 * accessible from the outside Internet.
 *
 * Usually it's better to use CreateServer() or CreateDatagramSocket() with a
 * nullptr address, to say "bind to all interfaces."
 *
 * The array of addresses returned from this is guaranteed to be
 * nullptr-terminated. You can also pass a pointer to an int, which will return
 * the final count, not counting the nullptr at the end of the array.
 *
 * @param num_addresses on exit, will be set to the number of addresses
 *                      returned. Can be nullptr.
 * @returns  one for each bindable address on the system on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_net 3.0.0.
 */
inline LocalAddressesArray GetLocalAddresses()
{
  int count;
  auto* addrs = CheckError(NET_GetLocalAddresses(&count));
  return LocalAddressesArray(reinterpret_cast<AddressRef*>(addrs), count);
}

/**
 * Free the results from GetLocalAddresses.
 *
 * This will unref all addresses in the array and free the array itself.
 *
 * Since addresses are reference counted, it is safe to keep any addresses you
 * want from this array even after calling this function, as long as you called
 * RefAddress() on them first.
 *
 * It is safe to pass a nullptr in here, it will be ignored.
 *
 * You most likely don't need to call this function directly, as the
 * LocalAddressesArray type will call it for you when the array goes out of
 * scope.
 *
 * @param addresses A pointer returned by GetLocalAddresses().
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_net 3.0.0.
 */
inline void FreeLocalAddresses(AddressRef* addresses)
{
  NET_FreeLocalAddresses(reinterpret_cast<AddressRaw*>(addresses));
}

inline void LocalAddressesArrayDeleter::operator()(AddressRef* addresses)
{
  FreeLocalAddresses(addresses);
}

/**
 * Base class to StreamSocket.
 *
 * @see StreamSocket
 */
struct StreamSocketBase : ResourceBaseT<StreamSocketRaw>
{
  using ResourceBaseT::ResourceBaseT;

  /**
   * Dispose of a previously-created stream socket.
   *
   * This will immediately disconnect the other side of the connection, if
   * necessary. Further attempts to read or write the socket on the remote end
   * will fail.
   *
   * This will _abandon_ any data queued for sending that hasn't made it to the
   * socket. If you need this data to arrive, you should wait for it to transmit
   * before destroying the socket with GetStreamSocketPendingWrites() or
   * WaitUntilStreamSocketDrained(). Any data that has arrived from the remote
   * end of the connection that hasn't been read yet is lost.
   *
   * @threadsafety You should not operate on the same socket from multiple
   *               threads at the same time without supplying a serialization
   *               mechanism. However, different threads may access different
   *               sockets at the same time without problems.
   *
   * @since This function is available since SDL_net 3.0.0.
   *
   * @sa CreateClient
   * @sa AcceptClient
   * @sa GetStreamSocketPendingWrites
   * @sa WaitUntilStreamSocketDrained
   */
  void Destroy();

  /**
   * Block until a stream socket has connected to a server.
   *
   * The StreamSocket objects returned by CreateClient take time to do their
   * work, so it does so _asynchronously_ instead of making your program wait an
   * indefinite amount of time.
   *
   * However, if you want your program to sleep until the connection is
   * complete, you can call this function.
   *
   * This function takes a timeout value, represented in milliseconds, of how
   * long to wait for resolution to complete. Specifying a timeout of -1
   * instructs the library to wait indefinitely, and a timeout of 0 just checks
   * the current status and returns immediately (and is functionally equivalent
   * to calling GetConnectionStatus).
   *
   * Connections can fail after some time (server took awhile to respond at all,
   * and then refused the connection outright), so be sure to check the result
   * of this function instead of assuming it worked!
   *
   * Once a connection is successfully made, the socket may read data from, or
   * write data to, the connected server.
   *
   * If you don't want your program to block, you can call GetConnectionStatus()
   * from time to time until you get a non-zero result.
   *
   * @param timeout Number of milliseconds to wait for resolution to complete.
   *                -1 to wait indefinitely, 0 to check once without waiting.
   * @returns SUCCESS if successfully connected, FAILURE if connection failed,
   *          WAITING if still connecting (this function timed out without
   *          resolution); if FAILURE, call GetError() for details.
   *
   * @threadsafety You should not operate on the same socket from multiple
   *               threads at the same time without supplying a serialization
   *               mechanism. However, different threads may access different
   *               socket at the same time without problems.
   *
   * @since This function is available since SDL_net 3.0.0.
   *
   * @sa GetConnectionStatus
   */
  Status WaitUntilConnected(Sint32 timeout);

  /**
   * Get the remote address of a stream socket.
   *
   * This reports the address of the remote side of a stream socket, which might
   * still be pending connnection.
   *
   * This adds a reference to the address; the caller _must_ call UnrefAddress()
   * when done with it.
   *
   * @returns the socket's remote address on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_net 3.0.0.
   */
  Address GetAddress();

  /**
   * Check if a stream socket is connected, without blocking.
   *
   * The StreamSocket objects returned by CreateClient take time to do negotiate
   * a connection to a server, so it does so _asynchronously_ instead of making
   * your program wait an indefinite amount of time.
   *
   * This function allows you to check the progress of that work without
   * blocking.
   *
   * Connection can fail after some time (server took a while to respond, and
   * then rejected the connection), so be sure to check the result of this
   * function instead of assuming it worked because it's non-zero!
   *
   * Once a connection is successfully made, the stream socket can be used to
   * send and receive data with the server.
   *
   * Note that if the connection succeeds, but later the connection is dropped,
   * this will still report the connection as successful, as it only deals with
   * the initial asynchronous work of getting connected; you'll know the
   * connection dropped later when your reads and writes report failures.
   *
   * @returns SUCCESS if successfully connected, FAILURE if connection failed,
   *          WAITING if still connecting; if FAILURE, call GetError() for
   *          details.
   *
   * @threadsafety You should not operate on the same socket from multiple
   *               threads at the same time without supplying a serialization
   *               mechanism. However, different threads may access different
   *               sockets at the same time without problems.
   *
   * @since This function is available since SDL_net 3.0.0.
   *
   * @sa WaitUntilConnected
   */
  Status GetConnectionStatus();

  /**
   * Send bytes over a stream socket to a remote system.
   *
   * Stream sockets are _reliable_, which means data sent over them will arrive
   * in the order it was transmitted, and the system will retransmit data as
   * necessary to ensure its delivery. Which is to say, short of catastrophic
   * failure, data will arrive, possibly with severe delays. Also, "catastrophic
   * failure" isn't an uncommon event.
   *
   * (This is opposed to Datagram sockets, which send chunks of data that might
   * arrive in any order, or not arrive at all, but you never wait for missing
   * chunks to show up.)
   *
   * Stream sockets are _bidirectional_; you can read and write from the same
   * stream, and the other end of the connection can, too.
   *
   * This call never blocks; if it can't send the data immediately, the library
   * will queue it for later transmission. You can use
   * GetStreamSocketPendingWrites() to see how much is still queued for later
   * transmission, or WaitUntilStreamSocketDrained() to block until all pending
   * data has been sent.
   *
   * If the connection has failed (remote side dropped us, or one of a million
   * other networking failures occurred), this function will report failure by
   * returning false. Stream sockets only report failure for unrecoverable
   * conditions; once a stream socket fails, you should assume it is no longer
   * usable and should destroy it with DestroyStreamSocket().
   *
   * @param buf a pointer to the data to send.
   * @param buflen the size of the data to send, in bytes.
   * @returns true if data sent or queued for transmission, false on failure;
   *          call GetError() for details.
   *
   * @threadsafety You should not operate on the same socket from multiple
   *               threads at the same time without supplying a serialization
   *               mechanism. However, different threads may access different
   *               sockets at the same time without problems.
   *
   * @since This function is available since SDL_net 3.0.0.
   *
   * @sa GetStreamSocketPendingWrites
   * @sa WaitUntilStreamSocketDrained
   * @sa ReadFromStreamSocket
   */
  bool WriteTo(const void* buf, int buflen);

  /**
   * Query bytes still pending transmission on a stream socket.
   *
   * If WriteToStreamSocket() couldn't send all its data immediately, it will
   * queue it to be sent later. This function lets the app see how much of that
   * queue is still pending to be sent.
   *
   * The library will try to send more queued data before reporting what's left,
   * but it will not block to do so.
   *
   * If the connection has failed (remote side dropped us, or one of a million
   * other networking failures occurred), this function will report failure by
   * returning -1. Stream sockets only report failure for unrecoverable
   * conditions; once a stream socket fails, you should assume it is no longer
   * usable and should destroy it with DestroyStreamSocket().
   *
   * @returns number of bytes still pending transmission, -1 on failure; call
   *          GetError() for details.
   *
   * @threadsafety You should not operate on the same socket from multiple
   *               threads at the same time without supplying a serialization
   *               mechanism. However, different threads may access different
   *               sockets at the same time without problems.
   *
   * @since This function is available since SDL_net 3.0.0.
   *
   * @sa WriteToStreamSocket
   * @sa WaitUntilStreamSocketDrained
   */
  int GetPendingWrites();

  /**
   * Block until all of a stream socket's pending data is sent.
   *
   * If WriteToStreamSocket() couldn't send all its data immediately, it will
   * queue it to be sent later. This function lets the app sleep until all the
   * data is transmitted.
   *
   * This function takes a timeout value, represented in milliseconds, of how
   * long to wait for transmission to complete. Specifying a timeout of -1
   * instructs the library to wait indefinitely, and a timeout of 0 just checks
   * the current status and returns immediately (and is functionally equivalent
   * to calling GetStreamSocketPendingWrites).
   *
   * If you don't want your program to block, you can call
   * GetStreamSocketPendingWrites from time to time until you get a result <= 0.
   *
   * If the connection has failed (remote side dropped us, or one of a million
   * other networking failures occurred), this function will report failure by
   * returning -1. Stream sockets only report failure for unrecoverable
   * conditions; once a stream socket fails, you should assume it is no longer
   * usable and should destroy it with DestroyStreamSocket().
   *
   * @param timeout Number of milliseconds to wait for draining to complete. -1
   *                to wait indefinitely, 0 to check once without waiting.
   * @returns number of bytes still pending transmission, -1 on failure; call
   *          GetError() for details.
   *
   * @threadsafety You should not operate on the same socket from multiple
   *               threads at the same time without supplying a serialization
   *               mechanism. However, different threads may access different
   *               sockets at the same time without problems.
   *
   * @since This function is available since SDL_net 3.0.0.
   *
   * @sa WriteToStreamSocket
   * @sa GetStreamSocketPendingWrites
   */
  int WaitUntilDrained(Sint32 timeout);

  /**
   * Receive bytes that a remote system sent to a stream socket.
   *
   * Stream sockets are _reliable_, which means data sent over them will arrive
   * in the order it was transmitted, and the system will retransmit data as
   * necessary to ensure its delivery. Which is to say, short of catastrophic
   * failure, data will arrive, possibly with severe delays. Also, "catastrophic
   * failure" isn't an uncommon event.
   *
   * (This is opposed to Datagram sockets, which send chunks of data that might
   * arrive in any order, or not arrive at all, but you never wait for missing
   * chunks to show up.)
   *
   * Stream sockets are _bidirectional_; you can read and write from the same
   * stream, and the other end of the connection can, too.
   *
   * This function returns data that has arrived for the stream socket that
   * hasn't been read yet. Data is provided in the order it was sent on the
   * remote side. This function may return less data than requested, depending
   * on what is available at the time, and also the app isn't required to read
   * all available data at once.
   *
   * This call never blocks; if no new data isn't available at the time of the
   * call, it returns 0 immediately. The caller can try again later.
   *
   * If the connection has failed (remote side dropped us, or one of a million
   * other networking failures occurred), this function will report failure by
   * returning -1. Stream sockets only report failure for unrecoverable
   * conditions; once a stream socket fails, you should assume it is no longer
   * usable and should destroy it with DestroyStreamSocket().
   *
   * @param buf a pointer to a buffer where received data will be collected.
   * @param buflen the size of the buffer pointed to by `buf`, in bytes. This is
   *               the maximum that will be read from the stream socket.
   * @returns number of bytes read from the stream socket (which can be less
   *          than `buflen` or zero if none available), -1 on failure; call
   *          GetError() for details.
   *
   * @threadsafety You should not operate on the same socket from multiple
   *               threads at the same time without supplying a serialization
   *               mechanism. However, different threads may access different
   *               sockets at the same time without problems.
   *
   * @since This function is available since SDL_net 3.0.0.
   *
   * @sa WriteToStreamSocket
   */
  int ReadFrom(void* buf, int buflen);

  /**
   * Enable simulated stream socket failures.
   *
   * Often times, testing a networked app on your development machine--which
   * might have a wired connection to a fast, reliable network service--won't
   * expose bugs that happen when networks intermittently fail in the real
   * world, when the wifi is flakey and firewalls get in the way.
   *
   * This function allows you to tell the library to pretend that some
   * percentage of stream socket data transmission will fail.
   *
   * Since stream sockets are reliable, failure in this case pretends that
   * packets are getting lost on the network, making the stream retransmit to
   * deal with it. To simulate this, the library will introduce some amount of
   * delay before it sends or receives data on the socket. The higher the
   * percentage, the more delay is introduced for bytes to make their way to
   * their final destination. The library may also decide to drop connections at
   * random, to simulate disasterous network conditions.
   *
   * Setting this to zero (the default) will disable the simulation. Setting to
   * 100 means _everything_ fails unconditionally and no further data will get
   * through (and perhaps your sockets eventually fail). At what percent the
   * system merely borders on unusable is left as an exercise to the app
   * developer.
   *
   * This is intended for debugging purposes, to simulate real-world conditions
   * that are various degrees of terrible. You probably should _not_ call this
   * in production code, where you'll likely see real failures anyhow.
   *
   * @param percent_loss A number between 0 and 100. Higher means more failures.
   *                     Zero to disable.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_net 3.0.0.
   */
  void SimulateStreamPacketLoss(int percent_loss);
};

/**
 * An object that represents a streaming connection to another system.
 *
 * This is meant to be a reliable, stream-oriented connection, such as TCP.
 *
 * Each StreamSocket represents a single connection between systems. Usually, a
 * client app will have one connection to a server app on a different computer,
 * and the server app might have many connections from different clients. Each
 * of these connections communicate over a separate stream socket.
 *
 * @since This datatype is available since SDL_net 3.0.0.
 *
 * @sa CreateClient
 * @sa WriteToStreamSocket
 * @sa ReadFromStreamSocket
 *
 * @cat resource
 */
struct StreamSocket : StreamSocketBase
{
  using StreamSocketBase::StreamSocketBase;

  /**
   * Constructs from raw StreamSocket.
   *
   * @param resource a StreamSocketRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit StreamSocket(StreamSocketRaw resource) noexcept
    : StreamSocketBase(resource)
  {
  }

  /// Copy constructor
  constexpr StreamSocket(const StreamSocket& other) = delete;

  /// Move constructor
  constexpr StreamSocket(StreamSocket&& other) noexcept
    : StreamSocket(other.release())
  {
  }

  /**
   * Begin connecting a socket as a client to a remote server.
   *
   * Each StreamSocket represents a single connection between systems. Usually,
   * a client app will have one connection to a server app on a different
   * computer, and the server app might have many connections from different
   * clients. Each of these connections communicate over a separate stream
   * socket.
   *
   * Connecting is an asynchronous operation; this function does not block, and
   * will return before the connection is complete. One has to then use
   * WaitUntilConnected() or GetConnectionStatus() to see when the operation has
   * completed, and if it was successful.
   *
   * Once connected, you can read and write data to the returned socket. Stream
   * sockets are a mode of _reliable_ transmission, which means data will be
   * received as a stream of bytes in the order you sent it. If there are
   * problems in transmission, the system will deal with protocol negotiation
   * and retransmission as necessary, transparent to your app, but this means
   * until data is available in the order sent, the remote side will not get any
   * new data. This is the tradeoff vs datagram sockets, where data can arrive
   * in any order, or not arrive at all, without waiting, but the sender will
   * not know.
   *
   * Stream sockets don't employ any protocol (above the TCP level), so they can
   * connect to servers that aren't using SDL_net, but if you want to speak any
   * protocol beyond an abritrary stream of bytes, such as HTTP, you'll have to
   * implement that yourself on top of the stream socket.
   *
   * This function will fail if `address` is not finished resolving.
   *
   * When you are done with this connection (whether it failed to connect or
   * not), you must dispose of it with DestroyStreamSocket().
   *
   * Unlike BSD sockets or WinSock, you specify the port as a normal integer;
   * you do not have to byteswap it into "network order," as the library will
   * handle that for you.
   *
   * There are currently no extra properties for creating a client, so `props`
   * should be zero. A future revision of SDL_net may add additional (optional)
   * properties.
   *
   * @param address the address of the remote server to connect to.
   * @param port the port on the remote server to connect to.
   * @param props properties of the new client. Specify zero for defaults.
   * @post  pending connection on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_net 3.0.0.
   *
   * @sa WaitUntilConnected
   * @sa GetConnectionStatus
   * @sa DestroyStreamSocket
   */
  StreamSocket(AddressRef address, Uint16 port, PropertiesRef props);

  /// Destructor
  ~StreamSocket() { NET_DestroyStreamSocket(get()); }

  /// Assignment operator.
  constexpr StreamSocket& operator=(StreamSocket&& other) noexcept
  {
    swap(*this, other);
    return *this;
  }

  /// Assignment operator.
  StreamSocket& operator=(const StreamSocket& other) = delete;
};

/**
 * Begin connecting a socket as a client to a remote server.
 *
 * Each StreamSocket represents a single connection between systems. Usually, a
 * client app will have one connection to a server app on a different computer,
 * and the server app might have many connections from different clients. Each
 * of these connections communicate over a separate stream socket.
 *
 * Connecting is an asynchronous operation; this function does not block, and
 * will return before the connection is complete. One has to then use
 * WaitUntilConnected() or GetConnectionStatus() to see when the operation has
 * completed, and if it was successful.
 *
 * Once connected, you can read and write data to the returned socket. Stream
 * sockets are a mode of _reliable_ transmission, which means data will be
 * received as a stream of bytes in the order you sent it. If there are problems
 * in transmission, the system will deal with protocol negotiation and
 * retransmission as necessary, transparent to your app, but this means until
 * data is available in the order sent, the remote side will not get any new
 * data. This is the tradeoff vs datagram sockets, where data can arrive in any
 * order, or not arrive at all, without waiting, but the sender will not know.
 *
 * Stream sockets don't employ any protocol (above the TCP level), so they can
 * connect to servers that aren't using SDL_net, but if you want to speak any
 * protocol beyond an abritrary stream of bytes, such as HTTP, you'll have to
 * implement that yourself on top of the stream socket.
 *
 * This function will fail if `address` is not finished resolving.
 *
 * When you are done with this connection (whether it failed to connect or not),
 * you must dispose of it with DestroyStreamSocket().
 *
 * Unlike BSD sockets or WinSock, you specify the port as a normal integer; you
 * do not have to byteswap it into "network order," as the library will handle
 * that for you.
 *
 * There are currently no extra properties for creating a client, so `props`
 * should be zero. A future revision of SDL_net may add additional (optional)
 * properties.
 *
 * @param address the address of the remote server to connect to.
 * @param port the port on the remote server to connect to.
 * @param props properties of the new client. Specify zero for defaults.
 * @returns  pending connection on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_net 3.0.0.
 *
 * @sa WaitUntilConnected
 * @sa GetConnectionStatus
 * @sa DestroyStreamSocket
 */
inline StreamSocket CreateClient(AddressRef address,
                                 Uint16 port,
                                 PropertiesRef props)
{
  return StreamSocket(address, port, props);
}

inline StreamSocket AddressBase::CreateClient(Uint16 port, PropertiesRef props)
{
  return StreamSocket(get(), port, props);
}

inline StreamSocket::StreamSocket(AddressRef address,
                                  Uint16 port,
                                  PropertiesRef props)
  : StreamSocket(CheckError(NET_CreateClient(address, port, props)))
{
}

/**
 * Block until a stream socket has connected to a server.
 *
 * The StreamSocket objects returned by CreateClient take time to do their work,
 * so it does so _asynchronously_ instead of making your program wait an
 * indefinite amount of time.
 *
 * However, if you want your program to sleep until the connection is complete,
 * you can call this function.
 *
 * This function takes a timeout value, represented in milliseconds, of how long
 * to wait for resolution to complete. Specifying a timeout of -1 instructs the
 * library to wait indefinitely, and a timeout of 0 just checks the current
 * status and returns immediately (and is functionally equivalent to calling
 * GetConnectionStatus).
 *
 * Connections can fail after some time (server took awhile to respond at all,
 * and then refused the connection outright), so be sure to check the result of
 * this function instead of assuming it worked!
 *
 * Once a connection is successfully made, the socket may read data from, or
 * write data to, the connected server.
 *
 * If you don't want your program to block, you can call GetConnectionStatus()
 * from time to time until you get a non-zero result.
 *
 * @param sock The StreamSocket object to wait on.
 * @param timeout Number of milliseconds to wait for resolution to complete. -1
 *                to wait indefinitely, 0 to check once without waiting.
 * @returns SUCCESS if successfully connected, FAILURE if connection failed,
 *          WAITING if still connecting (this function timed out without
 *          resolution); if FAILURE, call GetError() for details.
 *
 * @threadsafety You should not operate on the same socket from multiple threads
 *               at the same time without supplying a serialization mechanism.
 *               However, different threads may access different socket at the
 *               same time without problems.
 *
 * @since This function is available since SDL_net 3.0.0.
 *
 * @sa GetConnectionStatus
 */
inline Status WaitUntilConnected(StreamSocketRef sock, Sint32 timeout)
{
  return NET_WaitUntilConnected(sock, timeout);
}

inline Status StreamSocketBase::WaitUntilConnected(Sint32 timeout)
{
  return SDL::WaitUntilConnected(get(), timeout);
}

/**
 * Base class to Server.
 *
 * @see Server
 */
struct ServerBase : ResourceBaseT<ServerRaw>
{
  using ResourceBaseT::ResourceBaseT;

  /**
   * Dispose of a previously-created server.
   *
   * This will immediately disconnect any pending client connections that had
   * not yet been accepted, but will not disconnect any existing accepted
   * connections (which can still be used and must be destroyed separately).
   * Further attempts to make new connections to this server will fail on the
   * client side.
   *
   * @threadsafety You should not operate on the same server from multiple
   *               threads at the same time without supplying a serialization
   *               mechanism. However, different threads may access different
   *               servers at the same time without problems.
   *
   * @since This function is available since SDL_net 3.0.0.
   *
   * @sa CreateServer
   */
  void Destroy();

  /**
   * Create a stream socket for the next pending client connection.
   *
   * When a client connects to a server, their connection will be pending until
   * the server _accepts_ the connection. Once accepted, the server will be
   * given a stream socket to communicate with the client, and they can send
   * data to, and receive data from, each other.
   *
   * Unlike CreateClient, stream sockets returned from this function are already
   * connected and do not have to wait for the connection to complete, as server
   * acceptance is the final step of connecting.
   *
   * This function does not block. If there are no new connections pending, this
   * function will return true (for success, but `*client_stream` will be set to
   * nullptr. This is not an error and a common condition the app should expect.
   * In fact, this function should be called in a loop until this condition
   * occurs, so all pending connections are accepted in a single batch.
   *
   * If you want the server to sleep until there's a new connection, you can use
   * WaitUntilInputAvailable().
   *
   * When done with the newly-accepted client, you can disconnect and dispose of
   * the stream socket by calling DestroyStreamSocket().
   *
   * @param client_stream Will be set to a new stream socket if a connection was
   *                      pending, nullptr otherwise.
   * @throws Error on failure.
   *
   * @threadsafety You should not operate on the same server from multiple
   *               threads at the same time without supplying a serialization
   *               mechanism. However, different threads may access different
   *               servers at the same time without problems.
   *
   * @since This function is available since SDL_net 3.0.0.
   *
   * @sa WaitUntilInputAvailable
   * @sa DestroyStreamSocket
   */
  void AcceptClient(NET_StreamSocket** client_stream);
};

/**
 * The receiving end of a stream connection.
 *
 * This is an opaque datatype, to be treated by the app as a handle.
 *
 * Internally, this is what BSD sockets refers to as a "listen socket". Clients
 * attempt to connect to a server, and if the server accepts the connection,
 * will provide the app with a stream socket to send and receive data over that
 * connection.
 *
 * @since This datatype is available since SDL_net 3.0.0.
 *
 * @sa CreateServer
 *
 * @cat resource
 */
struct Server : ServerBase
{
  using ServerBase::ServerBase;

  /**
   * Constructs from raw Server.
   *
   * @param resource a ServerRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit Server(ServerRaw resource) noexcept
    : ServerBase(resource)
  {
  }

  /// Copy constructor
  constexpr Server(const Server& other) = delete;

  /// Move constructor
  constexpr Server(Server&& other) noexcept
    : Server(other.release())
  {
  }

  /**
   * Create a server, which listens for connections to accept.
   *
   * An app that initiates connection to a remote computer is called a "client,"
   * and the thing the client connects to is called a "server."
   *
   * Servers listen for and accept connections from clients, which spawns a new
   * stream socket on the server's end, which it can then send/receive data on.
   *
   * Use this function to create a server that will accept connections from
   * other systems.
   *
   * This function does not block, and is not asynchronous, as the system can
   * decide immediately if it can create a server or not. If this returns
   * success, you can immediately start accepting connections.
   *
   * You can specify an address to listen for connections on; this address must
   * be local to the system, and probably one returned by GetLocalAddresses(),
   * but almost always you just want to specify nullptr here, to listen on any
   * address available to the app.
   *
   * After creating a server, you get stream sockets to talk to incoming client
   * connections by calling AcceptClient().
   *
   * Stream sockets don't employ any protocol (above the TCP level), so they can
   * accept connections from clients that aren't using SDL_net, but if you want
   * to speak any protocol beyond an abritrary stream of bytes, such as HTTP,
   * you'll have to implement that yourself on top of the stream socket.
   *
   * Unlike BSD sockets or WinSock, you specify the port as a normal integer;
   * you do not have to byteswap it into "network order," as the library will
   * handle that for you.
   *
   * The caller may supply properties to customize behavior. This is optional,
   * and a value of zero for `props` will request defaults for all properties.
   *
   * These are the supported properties:
   *
   * - `prop.Server.REUSEADDR_BOOLEAN`: true if the server should be created
   *   even if a previous server has recently used this address. For various
   *   reasons, networks prefer that there be some delay between apps reusing
   *   the same address, but this can be problematic when iterating quickly, for
   *   software development purposes or just restarting a crashed service. This
   *   property defaults to true (although it should be noted that, at the
   *   operating system level, this defaults to false!). If this property is
   *   false and the OS feels that not enough time has elapsed, server creation
   *   will fail and this function will report an error.
   *
   * @param addr the _local_ address to listen for connections on, or nullptr.
   * @param port the port on the local address to listen for connections on.
   * @param props properties of the new server. Specify zero for defaults.
   * @post a new Server on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_net 3.0.0.
   *
   * @sa GetLocalAddresses
   * @sa AcceptClient
   * @sa DestroyServer
   */
  Server(AddressRef addr, Uint16 port, PropertiesRef props);

  /// Destructor
  ~Server() { NET_DestroyServer(get()); }

  /// Assignment operator.
  constexpr Server& operator=(Server&& other) noexcept
  {
    swap(*this, other);
    return *this;
  }

  /// Assignment operator.
  Server& operator=(const Server& other) = delete;
};

/**
 * Create a server, which listens for connections to accept.
 *
 * An app that initiates connection to a remote computer is called a "client,"
 * and the thing the client connects to is called a "server."
 *
 * Servers listen for and accept connections from clients, which spawns a new
 * stream socket on the server's end, which it can then send/receive data on.
 *
 * Use this function to create a server that will accept connections from other
 * systems.
 *
 * This function does not block, and is not asynchronous, as the system can
 * decide immediately if it can create a server or not. If this returns success,
 * you can immediately start accepting connections.
 *
 * You can specify an address to listen for connections on; this address must be
 * local to the system, and probably one returned by GetLocalAddresses(), but
 * almost always you just want to specify nullptr here, to listen on any address
 * available to the app.
 *
 * After creating a server, you get stream sockets to talk to incoming client
 * connections by calling AcceptClient().
 *
 * Stream sockets don't employ any protocol (above the TCP level), so they can
 * accept connections from clients that aren't using SDL_net, but if you want to
 * speak any protocol beyond an abritrary stream of bytes, such as HTTP, you'll
 * have to implement that yourself on top of the stream socket.
 *
 * Unlike BSD sockets or WinSock, you specify the port as a normal integer; you
 * do not have to byteswap it into "network order," as the library will handle
 * that for you.
 *
 * The caller may supply properties to customize behavior. This is optional, and
 * a value of zero for `props` will request defaults for all properties.
 *
 * These are the supported properties:
 *
 * - `prop.Server.REUSEADDR_BOOLEAN`: true if the server should be created even
 *   if a previous server has recently used this address. For various reasons,
 *   networks prefer that there be some delay between apps reusing the same
 *   address, but this can be problematic when iterating quickly, for software
 *   development purposes or just restarting a crashed service. This property
 *   defaults to true (although it should be noted that, at the operating system
 *   level, this defaults to false!). If this property is false and the OS feels
 *   that not enough time has elapsed, server creation will fail and this
 *   function will report an error.
 *
 * @param addr the _local_ address to listen for connections on, or nullptr.
 * @param port the port on the local address to listen for connections on.
 * @param props properties of the new server. Specify zero for defaults.
 * @returns a new Server on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_net 3.0.0.
 *
 * @sa GetLocalAddresses
 * @sa AcceptClient
 * @sa DestroyServer
 */
inline Server CreateServer(AddressRef addr, Uint16 port, PropertiesRef props)
{
  return Server(addr, port, props);
}

inline Server AddressBase::CreateServer(Uint16 port, PropertiesRef props)
{
  return Server(get(), port, props);
}

inline Server::Server(AddressRef addr, Uint16 port, PropertiesRef props)
  : Server(CheckError(NET_CreateServer(addr, port, props)))
{
}

namespace prop::Server {

constexpr auto REUSEADDR_BOOLEAN =
  NET_PROP_SERVER_REUSEADDR_BOOLEAN; ///< Reuseaddr enabled.

} // namespace prop::Server

/**
 * Create a stream socket for the next pending client connection.
 *
 * When a client connects to a server, their connection will be pending until
 * the server _accepts_ the connection. Once accepted, the server will be given
 * a stream socket to communicate with the client, and they can send data to,
 * and receive data from, each other.
 *
 * Unlike CreateClient, stream sockets returned from this function are already
 * connected and do not have to wait for the connection to complete, as server
 * acceptance is the final step of connecting.
 *
 * This function does not block. If there are no new connections pending, this
 * function will return true (for success, but `*client_stream` will be set to
 * nullptr. This is not an error and a common condition the app should expect.
 * In fact, this function should be called in a loop until this condition
 * occurs, so all pending connections are accepted in a single batch.
 *
 * If you want the server to sleep until there's a new connection, you can use
 * WaitUntilInputAvailable().
 *
 * When done with the newly-accepted client, you can disconnect and dispose of
 * the stream socket by calling DestroyStreamSocket().
 *
 * @param server the server object to check for pending connections.
 * @param client_stream Will be set to a new stream socket if a connection was
 *                      pending, nullptr otherwise.
 * @throws Error on failure.
 *
 * @threadsafety You should not operate on the same server from multiple threads
 *               at the same time without supplying a serialization mechanism.
 *               However, different threads may access different servers at the
 *               same time without problems.
 *
 * @since This function is available since SDL_net 3.0.0.
 *
 * @sa WaitUntilInputAvailable
 * @sa DestroyStreamSocket
 */
inline void AcceptClient(ServerRef server, NET_StreamSocket** client_stream)
{
  CheckError(NET_AcceptClient(server, client_stream));
}

inline void ServerBase::AcceptClient(NET_StreamSocket** client_stream)
{
  SDL::AcceptClient(get(), client_stream);
}

/**
 * Dispose of a previously-created server.
 *
 * This will immediately disconnect any pending client connections that had not
 * yet been accepted, but will not disconnect any existing accepted connections
 * (which can still be used and must be destroyed separately). Further attempts
 * to make new connections to this server will fail on the client side.
 *
 * @param server server to destroy.
 *
 * @threadsafety You should not operate on the same server from multiple threads
 *               at the same time without supplying a serialization mechanism.
 *               However, different threads may access different servers at the
 *               same time without problems.
 *
 * @since This function is available since SDL_net 3.0.0.
 *
 * @sa CreateServer
 */
inline void DestroyServer(ServerRaw server) { NET_DestroyServer(server); }

inline void ServerBase::Destroy() { DestroyServer(release()); }

/**
 * Get the remote address of a stream socket.
 *
 * This reports the address of the remote side of a stream socket, which might
 * still be pending connnection.
 *
 * This adds a reference to the address; the caller _must_ call UnrefAddress()
 * when done with it.
 *
 * @param sock the stream socket to query.
 * @returns the socket's remote address on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_net 3.0.0.
 */
inline Address GetStreamSocketAddress(StreamSocketRef sock)
{
  return Address(CheckError(NET_GetStreamSocketAddress(sock)));
}

inline Address StreamSocketBase::GetAddress()
{
  return SDL::GetStreamSocketAddress(get());
}

/**
 * Check if a stream socket is connected, without blocking.
 *
 * The StreamSocket objects returned by CreateClient take time to do negotiate a
 * connection to a server, so it does so _asynchronously_ instead of making your
 * program wait an indefinite amount of time.
 *
 * This function allows you to check the progress of that work without blocking.
 *
 * Connection can fail after some time (server took a while to respond, and then
 * rejected the connection), so be sure to check the result of this function
 * instead of assuming it worked because it's non-zero!
 *
 * Once a connection is successfully made, the stream socket can be used to send
 * and receive data with the server.
 *
 * Note that if the connection succeeds, but later the connection is dropped,
 * this will still report the connection as successful, as it only deals with
 * the initial asynchronous work of getting connected; you'll know the
 * connection dropped later when your reads and writes report failures.
 *
 * @param sock the stream socket to query.
 * @returns SUCCESS if successfully connected, FAILURE if connection failed,
 *          WAITING if still connecting; if FAILURE, call GetError() for
 *          details.
 *
 * @threadsafety You should not operate on the same socket from multiple threads
 *               at the same time without supplying a serialization mechanism.
 *               However, different threads may access different sockets at the
 *               same time without problems.
 *
 * @since This function is available since SDL_net 3.0.0.
 *
 * @sa WaitUntilConnected
 */
inline Status GetConnectionStatus(StreamSocketRef sock)
{
  return NET_GetConnectionStatus(sock);
}

inline Status StreamSocketBase::GetConnectionStatus()
{
  return SDL::GetConnectionStatus(get());
}

/**
 * Send bytes over a stream socket to a remote system.
 *
 * Stream sockets are _reliable_, which means data sent over them will arrive in
 * the order it was transmitted, and the system will retransmit data as
 * necessary to ensure its delivery. Which is to say, short of catastrophic
 * failure, data will arrive, possibly with severe delays. Also, "catastrophic
 * failure" isn't an uncommon event.
 *
 * (This is opposed to Datagram sockets, which send chunks of data that might
 * arrive in any order, or not arrive at all, but you never wait for missing
 * chunks to show up.)
 *
 * Stream sockets are _bidirectional_; you can read and write from the same
 * stream, and the other end of the connection can, too.
 *
 * This call never blocks; if it can't send the data immediately, the library
 * will queue it for later transmission. You can use
 * GetStreamSocketPendingWrites() to see how much is still queued for later
 * transmission, or WaitUntilStreamSocketDrained() to block until all pending
 * data has been sent.
 *
 * If the connection has failed (remote side dropped us, or one of a million
 * other networking failures occurred), this function will report failure by
 * returning false. Stream sockets only report failure for unrecoverable
 * conditions; once a stream socket fails, you should assume it is no longer
 * usable and should destroy it with DestroyStreamSocket().
 *
 * @param sock the stream socket to send data through.
 * @param buf a pointer to the data to send.
 * @param buflen the size of the data to send, in bytes.
 * @returns true if data sent or queued for transmission, false on failure; call
 *          GetError() for details.
 *
 * @threadsafety You should not operate on the same socket from multiple threads
 *               at the same time without supplying a serialization mechanism.
 *               However, different threads may access different sockets at the
 *               same time without problems.
 *
 * @since This function is available since SDL_net 3.0.0.
 *
 * @sa GetStreamSocketPendingWrites
 * @sa WaitUntilStreamSocketDrained
 * @sa ReadFromStreamSocket
 */
inline bool WriteToStreamSocket(StreamSocketRef sock,
                                const void* buf,
                                int buflen)
{
  return NET_WriteToStreamSocket(sock, buf, buflen);
}

inline bool StreamSocketBase::WriteTo(const void* buf, int buflen)
{
  return SDL::WriteToStreamSocket(get(), buf, buflen);
}

/**
 * Query bytes still pending transmission on a stream socket.
 *
 * If WriteToStreamSocket() couldn't send all its data immediately, it will
 * queue it to be sent later. This function lets the app see how much of that
 * queue is still pending to be sent.
 *
 * The library will try to send more queued data before reporting what's left,
 * but it will not block to do so.
 *
 * If the connection has failed (remote side dropped us, or one of a million
 * other networking failures occurred), this function will report failure by
 * returning -1. Stream sockets only report failure for unrecoverable
 * conditions; once a stream socket fails, you should assume it is no longer
 * usable and should destroy it with DestroyStreamSocket().
 *
 * @param sock the stream socket to query.
 * @returns number of bytes still pending transmission, -1 on failure; call
 *          GetError() for details.
 *
 * @threadsafety You should not operate on the same socket from multiple threads
 *               at the same time without supplying a serialization mechanism.
 *               However, different threads may access different sockets at the
 *               same time without problems.
 *
 * @since This function is available since SDL_net 3.0.0.
 *
 * @sa WriteToStreamSocket
 * @sa WaitUntilStreamSocketDrained
 */
inline int GetStreamSocketPendingWrites(StreamSocketRef sock)
{
  return NET_GetStreamSocketPendingWrites(sock);
}

inline int StreamSocketBase::GetPendingWrites()
{
  return SDL::GetStreamSocketPendingWrites(get());
}

/**
 * Block until all of a stream socket's pending data is sent.
 *
 * If WriteToStreamSocket() couldn't send all its data immediately, it will
 * queue it to be sent later. This function lets the app sleep until all the
 * data is transmitted.
 *
 * This function takes a timeout value, represented in milliseconds, of how long
 * to wait for transmission to complete. Specifying a timeout of -1 instructs
 * the library to wait indefinitely, and a timeout of 0 just checks the current
 * status and returns immediately (and is functionally equivalent to calling
 * GetStreamSocketPendingWrites).
 *
 * If you don't want your program to block, you can call
 * GetStreamSocketPendingWrites from time to time until you get a result <= 0.
 *
 * If the connection has failed (remote side dropped us, or one of a million
 * other networking failures occurred), this function will report failure by
 * returning -1. Stream sockets only report failure for unrecoverable
 * conditions; once a stream socket fails, you should assume it is no longer
 * usable and should destroy it with DestroyStreamSocket().
 *
 * @param sock the stream socket to wait on.
 * @param timeout Number of milliseconds to wait for draining to complete. -1 to
 *                wait indefinitely, 0 to check once without waiting.
 * @returns number of bytes still pending transmission, -1 on failure; call
 *          GetError() for details.
 *
 * @threadsafety You should not operate on the same socket from multiple threads
 *               at the same time without supplying a serialization mechanism.
 *               However, different threads may access different sockets at the
 *               same time without problems.
 *
 * @since This function is available since SDL_net 3.0.0.
 *
 * @sa WriteToStreamSocket
 * @sa GetStreamSocketPendingWrites
 */
inline int WaitUntilStreamSocketDrained(StreamSocketRef sock, Sint32 timeout)
{
  return NET_WaitUntilStreamSocketDrained(sock, timeout);
}

inline int StreamSocketBase::WaitUntilDrained(Sint32 timeout)
{
  return SDL::WaitUntilStreamSocketDrained(get(), timeout);
}

/**
 * Receive bytes that a remote system sent to a stream socket.
 *
 * Stream sockets are _reliable_, which means data sent over them will arrive in
 * the order it was transmitted, and the system will retransmit data as
 * necessary to ensure its delivery. Which is to say, short of catastrophic
 * failure, data will arrive, possibly with severe delays. Also, "catastrophic
 * failure" isn't an uncommon event.
 *
 * (This is opposed to Datagram sockets, which send chunks of data that might
 * arrive in any order, or not arrive at all, but you never wait for missing
 * chunks to show up.)
 *
 * Stream sockets are _bidirectional_; you can read and write from the same
 * stream, and the other end of the connection can, too.
 *
 * This function returns data that has arrived for the stream socket that hasn't
 * been read yet. Data is provided in the order it was sent on the remote side.
 * This function may return less data than requested, depending on what is
 * available at the time, and also the app isn't required to read all available
 * data at once.
 *
 * This call never blocks; if no new data isn't available at the time of the
 * call, it returns 0 immediately. The caller can try again later.
 *
 * If the connection has failed (remote side dropped us, or one of a million
 * other networking failures occurred), this function will report failure by
 * returning -1. Stream sockets only report failure for unrecoverable
 * conditions; once a stream socket fails, you should assume it is no longer
 * usable and should destroy it with DestroyStreamSocket().
 *
 * @param sock the stream socket to receive data from.
 * @param buf a pointer to a buffer where received data will be collected.
 * @param buflen the size of the buffer pointed to by `buf`, in bytes. This is
 *               the maximum that will be read from the stream socket.
 * @returns number of bytes read from the stream socket (which can be less than
 *          `buflen` or zero if none available), -1 on failure; call GetError()
 *          for details.
 *
 * @threadsafety You should not operate on the same socket from multiple threads
 *               at the same time without supplying a serialization mechanism.
 *               However, different threads may access different sockets at the
 *               same time without problems.
 *
 * @since This function is available since SDL_net 3.0.0.
 *
 * @sa WriteToStreamSocket
 */
inline int ReadFromStreamSocket(StreamSocketRef sock, void* buf, int buflen)
{
  return NET_ReadFromStreamSocket(sock, buf, buflen);
}

inline int StreamSocketBase::ReadFrom(void* buf, int buflen)
{
  return SDL::ReadFromStreamSocket(get(), buf, buflen);
}

/**
 * Enable simulated stream socket failures.
 *
 * Often times, testing a networked app on your development machine--which might
 * have a wired connection to a fast, reliable network service--won't expose
 * bugs that happen when networks intermittently fail in the real world, when
 * the wifi is flakey and firewalls get in the way.
 *
 * This function allows you to tell the library to pretend that some percentage
 * of stream socket data transmission will fail.
 *
 * Since stream sockets are reliable, failure in this case pretends that packets
 * are getting lost on the network, making the stream retransmit to deal with
 * it. To simulate this, the library will introduce some amount of delay before
 * it sends or receives data on the socket. The higher the percentage, the more
 * delay is introduced for bytes to make their way to their final destination.
 * The library may also decide to drop connections at random, to simulate
 * disasterous network conditions.
 *
 * Setting this to zero (the default) will disable the simulation. Setting to
 * 100 means _everything_ fails unconditionally and no further data will get
 * through (and perhaps your sockets eventually fail). At what percent the
 * system merely borders on unusable is left as an exercise to the app
 * developer.
 *
 * This is intended for debugging purposes, to simulate real-world conditions
 * that are various degrees of terrible. You probably should _not_ call this in
 * production code, where you'll likely see real failures anyhow.
 *
 * @param sock The socket to set a failure rate on.
 * @param percent_loss A number between 0 and 100. Higher means more failures.
 *                     Zero to disable.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_net 3.0.0.
 */
inline void SimulateStreamPacketLoss(StreamSocketRef sock, int percent_loss)
{
  NET_SimulateStreamPacketLoss(sock, percent_loss);
}

inline void StreamSocketBase::SimulateStreamPacketLoss(int percent_loss)
{
  SDL::SimulateStreamPacketLoss(get(), percent_loss);
}

/**
 * Dispose of a previously-created stream socket.
 *
 * This will immediately disconnect the other side of the connection, if
 * necessary. Further attempts to read or write the socket on the remote end
 * will fail.
 *
 * This will _abandon_ any data queued for sending that hasn't made it to the
 * socket. If you need this data to arrive, you should wait for it to transmit
 * before destroying the socket with GetStreamSocketPendingWrites() or
 * WaitUntilStreamSocketDrained(). Any data that has arrived from the remote end
 * of the connection that hasn't been read yet is lost.
 *
 * @param sock stream socket to destroy.
 *
 * @threadsafety You should not operate on the same socket from multiple threads
 *               at the same time without supplying a serialization mechanism.
 *               However, different threads may access different sockets at the
 *               same time without problems.
 *
 * @since This function is available since SDL_net 3.0.0.
 *
 * @sa CreateClient
 * @sa AcceptClient
 * @sa GetStreamSocketPendingWrites
 * @sa WaitUntilStreamSocketDrained
 */
inline void DestroyStreamSocket(StreamSocketRaw sock)
{
  NET_DestroyStreamSocket(sock);
}

inline void StreamSocketBase::Destroy() { DestroyStreamSocket(release()); }

/**
 * Base class to DatagramSocket.
 *
 * @see DatagramSocket
 */
struct DatagramSocketBase : ResourceBaseT<DatagramSocketRaw>
{
  using ResourceBaseT::ResourceBaseT;

  /**
   * Dispose of a previously-created datagram socket.
   *
   * This will _abandon_ any data queued for sending that hasn't made it to the
   * socket. If you need this data to arrive, you should wait for confirmation
   * from the remote computer in some form that you devise yourself. Queued data
   * is not guaranteed to arrive even if the library made efforts to transmit it
   * here.
   *
   * Any data that has arrived from the remote end of the connection that hasn't
   * been read yet is lost.
   *
   * @threadsafety You should not operate on the same socket from multiple
   *               threads at the same time without supplying a serialization
   *               mechanism. However, different threads may access different
   *               sockets at the same time without problems.
   *
   * @since This function is available since SDL_net 3.0.0.
   *
   * @sa CreateDatagramSocket
   * @sa SendDatagram
   * @sa ReceiveDatagram
   */
  void Destroy();

  /**
   * Send a new packet over a datagram socket to a remote system.
   *
   * Datagram sockets send packets of data. They either arrive as complete
   * packets or they don't arrive at all, as opposed to stream sockets, where
   * individual bytes might trickle in as they attempt to reliably deliver a
   * stream of data.
   *
   * Datagram packets might arrive in a different order than you sent them, or
   * they may just be lost while travelling across the network. You have to plan
   * for this. As an added confusion, since SDL_net might send the same packet
   * on multiple interfaces, you might get duplicate packets, possibly from
   * different network addresses. You have to plan for this, too.
   *
   * You can send to any address and port on the network, but there has to be a
   * datagram socket waiting for the data on the other side for the packet not
   * to be lost.
   *
   * General wisdom is that you shouldn't send a packet larger than 1500 bytes
   * over the Internet, as bad routers might fragment or lose larger ones, but
   * this limit is not hardcoded into SDL_net and in good conditions you might
   * be able to send significantly more.
   *
   * This call never blocks; if it can't send the data immediately, the library
   * will queue it for later transmission. There is no query to see what is
   * still queued, as datagram transmission is unreliable, so you should never
   * assume anything about queued data.
   *
   * If there's a fatal error, this function will return false. Datagram sockets
   * generally won't report failures, because there is no state like a
   * "connection" to fail at this level, but may report failure for
   * unrecoverable system-level conditions; once a datagram socket fails, you
   * should assume it is no longer usable and should destroy it with
   * SDL_DestroyDatagramSocket().
   *
   * Sending to a nullptr address is treated as a request to broadcast a packet.
   * Note that this will report failure immediately if the socket was not
   * created with broadcast permission. Broadcast packets are (more or less)
   * sent to every machine on the LAN, unconditionally.
   *
   * **WARNING**: It is possible to build a game where everyone is playing on
   * the same LAN, and every player is simply broadcasting packets. This is
   * absolutely the wrong thing to do, however. Broadcast packets go to every
   * device on the LAN, whether they want them or not. The game DOOM, in its
   * heyday, was capable of [bringing entire networks to their
   * knees](https://doomwiki.org/wiki/Doom_in_workplaces) , as many players on
   * the same network would all be broadcasting relentlessly.
   *
   * In practice, broadcasting sparingly can be useful for certain
   * functionality: a LAN-only client broadcasting a few packets to ask for
   * available servers, and running servers replying directly to that client
   * without broadcasting at all, is reasonable and safe. Once clients and
   * servers have found each other, they can communicate directly without any
   * broadcasting at all. For peer-to-peer games, once connection is
   * established, it's better to either send unique packets to each known
   * player, or use a multicasting (which works like broadcast, but only routes
   * packets to devices that are explicitly listening for it).
   *
   * With IPv6, which doesn't support broadcasts, broadcasting is faked with
   * multicast to the all-nodes link-local multicast group, ff02::1, either on a
   * specific interface or letting the OS choose the default. Other protocols
   * might fake broadcast operations in similar ways in the future.
   *
   * @param address the Address object address. May be nullptr to broadcast.
   * @param port the address port.
   * @param buf a pointer to the data to send as a single packet.
   * @param buflen the size of the data to send, in bytes.
   * @returns true if data sent or queued for transmission, false on failure;
   *          call GetError() for details.
   *
   * @threadsafety You should not operate on the same socket from multiple
   *               threads at the same time without supplying a serialization
   *               mechanism. However, different threads may access different
   *               sockets at the same time without problems.
   *
   * @since This function is available since SDL_net 3.0.0.
   *
   * @sa ReceiveDatagram
   */
  bool SendDatagram(AddressRef address,
                    Uint16 port,
                    const void* buf,
                    int buflen);

  /**
   * Receive a new packet that a remote system sent to a datagram socket.
   *
   * Datagram sockets send packets of data. They either arrive as complete
   * packets or they don't arrive at all, so you'll never receive half a packet.
   *
   * This call never blocks; if no new data is available at the time of the
   * call, it returns true immediately. The caller can try again later.
   *
   * On a successful call to this function, it returns true, even if no new
   * packets are available, so you should check for a successful return and a
   * non-nullptr value in `*dgram` to decide if a new packet is available.
   *
   * You must pass received packets to DestroyDatagram when you are done with
   * them. If you want to save the sender's address past this time, it is safe
   * to call RefAddress() on the address and hold onto the pointer, so long as
   * you call UnrefAddress() on it when you are done with it.
   *
   * Since datagrams can arrive from any address or port on the network without
   * prior warning, this information is available in the Datagram object that is
   * provided by this function, and this is the only way to know who to reply
   * to. Even if you aren't acting as a "server," packets can still arrive at
   * your socket if someone sends one.
   *
   * If there's a fatal error, this function will return false. Datagram sockets
   * generally won't report failures, because there is no state like a
   * "connection" to fail at this level, but may report failure for
   * unrecoverable system-level conditions; once a datagram socket fails, you
   * should assume it is no longer usable and should destroy it with
   * SDL_DestroyDatagramSocket().
   *
   * @param dgram a reference to the datagram packet object.
   * @returns true if data sent or queued for transmission, false on failure;
   *          call GetError() for details.
   *
   * @threadsafety You should not operate on the same socket from multiple
   *               threads at the same time without supplying a serialization
   *               mechanism. However, different threads may access different
   *               sockets at the same time without problems.
   *
   * @since This function is available since SDL_net 3.0.0.
   *
   * @sa SendDatagram
   * @sa DestroyDatagram
   */
  bool ReceiveDatagram(Datagram& dgram);

  /**
   * Receive a new packet that a remote system sent to a datagram socket.
   *
   * Datagram sockets send packets of data. They either arrive as complete
   * packets or they don't arrive at all, so you'll never receive half a packet.
   *
   * This call never blocks; if no new data is available at the time of the
   * call, it returns true immediately. The caller can try again later.
   *
   * On a successful call to this function, it returns true, even if no new
   * packets are available, so you should check for a successful return and a
   * non-nullptr value in `*dgram` to decide if a new packet is available.
   *
   * You must pass received packets to DestroyDatagram when you are done with
   * them. If you want to save the sender's address past this time, it is safe
   * to call RefAddress() on the address and hold onto the pointer, so long as
   * you call UnrefAddress() on it when you are done with it.
   *
   * Since datagrams can arrive from any address or port on the network without
   * prior warning, this information is available in the Datagram object that is
   * provided by this function, and this is the only way to know who to reply
   * to. Even if you aren't acting as a "server," packets can still arrive at
   * your socket if someone sends one.
   *
   * If there's a fatal error, this function will return false. Datagram sockets
   * generally won't report failures, because there is no state like a
   * "connection" to fail at this level, but may report failure for
   * unrecoverable system-level conditions; once a datagram socket fails, you
   * should assume it is no longer usable and should destroy it with
   * SDL_DestroyDatagramSocket().
   *
   * @return a valid Datagram object if data sent or queued for transmission,
   *         nullptr on failure; call GetError() for details.
   *
   * @threadsafety You should not operate on the same socket from multiple
   *               threads at the same time without supplying a serialization
   *               mechanism. However, different threads may access different
   *               sockets at the same time without problems.
   *
   * @since This function is available since SDL_net 3.0.0.
   *
   * @sa SendDatagram
   * @sa DestroyDatagram
   */
  Datagram ReceiveDatagram();

  /**
   * Enable simulated datagram socket failures.
   *
   * Often times, testing a networked app on your development machine--which
   * might have a wired connection to a fast, reliable network service--won't
   * expose bugs that happen when networks intermittently fail in the real
   * world, when the wifi is flakey and firewalls get in the way.
   *
   * This function allows you to tell the library to pretend that some
   * percentage of datagram socket data transmission will fail.
   *
   * The library will randomly lose packets (both incoming and outgoing) at an
   * average matching `percent_loss`. Setting this to zero (the default) will
   * disable the simulation. Setting to 100 means _everything_ fails
   * unconditionally and no further data will get through. At what percent the
   * system merely borders on unusable is left as an exercise to the app
   * developer.
   *
   * This is intended for debugging purposes, to simulate real-world conditions
   * that are various degrees of terrible. You probably should _not_ call this
   * in production code, where you'll likely see real failures anyhow.
   *
   * @param percent_loss A number between 0 and 100. Higher means more failures.
   *                     Zero to disable.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_net 3.0.0.
   */
  void SimulateDatagramPacketLoss(int percent_loss);
};

/**
 * An object that represents a datagram connection to another system.
 *
 * This is meant to be an unreliable, packet-oriented connection, such as UDP.
 *
 * Datagram sockets follow different rules than stream sockets. They are not a
 * reliable stream of bytes but rather packets, they are not limited to talking
 * to a single other remote system, they do not maintain a single "connection"
 * that can be dropped, and they are more nimble about network failures at the
 * expense of being more complex to use. What makes sense for your app depends
 * entirely on what your app is trying to accomplish.
 *
 * Generally the idea of a datagram socket is that you send data one chunk
 * ("packet") at a time to any address you want, and it arrives whenever it gets
 * there, even if later packets get there first, and maybe it doesn't get there
 * at all, and you don't know when anything of this happens by default.
 *
 * @since This datatype is available since SDL_net 3.0.0.
 *
 * @sa CreateDatagramSocket
 * @sa SendDatagram
 * @sa ReceiveDatagram
 *
 * @cat resource
 */
struct DatagramSocket : DatagramSocketBase
{
  using DatagramSocketBase::DatagramSocketBase;

  /**
   * Constructs from raw DatagramSocket.
   *
   * @param resource a DatagramSocketRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit DatagramSocket(DatagramSocketRaw resource) noexcept
    : DatagramSocketBase(resource)
  {
  }

  /// Copy constructor
  constexpr DatagramSocket(const DatagramSocket& other) = delete;

  /// Move constructor
  constexpr DatagramSocket(DatagramSocket&& other) noexcept
    : DatagramSocket(other.release())
  {
  }

  /**
   * Create and bind a new datagram socket.
   *
   * Datagram sockets follow different rules than stream sockets. They are not a
   * reliable stream of bytes but rather packets, they are not limited to
   * talking to a single other remote system, they do not maintain a single
   * "connection" that can be dropped, and they are more nimble about network
   * failures at the expense of being more complex to use. What makes sense for
   * your app depends entirely on what your app is trying to accomplish.
   *
   * Generally the idea of a datagram socket is that you send data one chunk
   * ("packet") at a time to any address you want, and it arrives whenever it
   * gets there, even if later packets get there first, and maybe it doesn't get
   * there at all, and you don't know when anything of this happens by default.
   *
   * This function creates a new datagram socket.
   *
   * This function does not block, and is not asynchronous, as the system can
   * decide immediately if it can create a socket or not. If this returns
   * success, you can immediately start talking to the network.
   *
   * You can specify an address to listen for connections on; this address must
   * be local to the system, and probably one returned by GetLocalAddresses(),
   * but almost always you just want to specify nullptr here, to listen on any
   * address available to the app.
   *
   * If you need to bind to a specific port (like a server), you should specify
   * it in the `port` argument; datagram servers should do this, so they can be
   * reached at a well-known port. If you only plan to initiate communications
   * (like a client), you should specify 0 and let the system pick an unused
   * port. Only one process can bind to a specific port at a time, so if you
   * aren't acting as a server, you should choose 0. Datagram sockets can send
   * individual packets to any port, so this just declares where data will
   * arrive for your socket.
   *
   * Datagram sockets don't employ any protocol (above the UDP level), so they
   * can talk to apps that aren't using SDL_net, but if you want to speak any
   * protocol beyond arbitrary packets of bytes, such as WebRTC, you'll have to
   * implement that yourself on top of the stream socket.
   *
   * Unlike BSD sockets or WinSock, you specify the port as a normal integer;
   * you do not have to byteswap it into "network order," as the library will
   * handle that for you.
   *
   * The caller may supply properties to customize behavior. This is optional,
   * and a value of zero for `props` will request defaults for all properties.
   *
   * These are the supported properties:
   *
   * - `prop.DatagramSocket.REUSEADDR_BOOLEAN`: true if the socket should be
   *   created even if a previous socket has recently used this address. For
   *   various reasons, networks prefer that there be some delay between apps
   *   reusing the same address, but this can be problematic when iterating
   *   quickly, for software development purposes or just restarting a crashed
   *   service. This property defaults to true (although it should be noted
   *   that, at the operating system level, this defaults to false!). If this
   *   property is false and the OS feels that not enough time has elapsed,
   *   socket creation will fail and this function will report an error.
   * - `prop.DatagramSocket.ALLOW_BROADCAST_BOOLEAN`: true if the socket should
   *   allow broadcasting. At the lower level, this will set `SO_BROADCAST` for
   *   IPv4 sockets, to allow sending to the subnet's broadcast address at the
   *   OS level. For IPv6, it'll join the all-nodes link-local multicast group,
   *   ff02::1, allowing sending and receiving there, more or less simulating
   *   the usual IPv4 broadcast semantics. Other protocols take similar
   *   approaches. If you do not intend to send or receive broadcast packets on
   *   this socket, set this property to false, or omit it, as it defaults to
   *   false. Note: IPv4 will still be able to receive broadcast packets without
   *   this option, but IPv6 will not. Also see notes about sending to a
   *   broadcast address in SendDatagram().
   *
   * @param addr the local address to listen for connections on, or nullptr to
   *             listen on all available local addresses.
   * @param port the port on the local address to listen for connections on, or
   *             zero for the system to decide.
   * @param props properties of the new socket. Specify zero for defaults.
   * @post a new DatagramSocket on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_net 3.0.0.
   *
   * @sa GetLocalAddresses
   * @sa DestroyDatagramSocket
   */
  DatagramSocket(AddressRef addr, Uint16 port, PropertiesRef props);

  /// Destructor
  ~DatagramSocket() { NET_DestroyDatagramSocket(get()); }

  /// Assignment operator.
  constexpr DatagramSocket& operator=(DatagramSocket&& other) noexcept
  {
    swap(*this, other);
    return *this;
  }

  /// Assignment operator.
  DatagramSocket& operator=(const DatagramSocket& other) = delete;
};

/**
 * Base class to Datagram.
 *
 * @see Datagram
 */
struct DatagramBase : ResourceBaseT<DatagramRaw, DatagramRawConst>
{
  using ResourceBaseT::ResourceBaseT;

  /// Converts to DatagramConstRef
  constexpr operator DatagramConstRef() const noexcept { return get(); }

  /**
   * Dispose of a datagram packet previously received.
   *
   * You must pass packets received through ReceiveDatagram to this function
   * when you are done with them. This will free resources used by this packet
   * and unref its Address.
   *
   * If you want to save the sender's address from the packet past this time, it
   * is safe to call RefAddress() on the address and hold onto its pointer, so
   * long as you call UnrefAddress() on it when you are done with it.
   *
   * Once you call this function, the datagram pointer becomes invalid and
   * should not be used again by the app.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_net 3.0.0.
   */
  void Destroy();

  /**
   * Receive a new packet that a remote system sent to a datagram socket.
   *
   * Datagram sockets send packets of data. They either arrive as complete
   * packets or they don't arrive at all, so you'll never receive half a packet.
   *
   * This call never blocks; if no new data is available at the time of the
   * call, it returns true immediately. The caller can try again later.
   *
   * On a successful call to this function, it returns true, even if no new
   * packets are available, so you should check for a successful return and a
   * non-nullptr value in `*dgram` to decide if a new packet is available.
   *
   * You must pass received packets to DestroyDatagram when you are done with
   * them. If you want to save the sender's address past this time, it is safe
   * to call RefAddress() on the address and hold onto the pointer, so long as
   * you call UnrefAddress() on it when you are done with it.
   *
   * Since datagrams can arrive from any address or port on the network without
   * prior warning, this information is available in the Datagram object that is
   * provided by this function, and this is the only way to know who to reply
   * to. Even if you aren't acting as a "server," packets can still arrive at
   * your socket if someone sends one.
   *
   * If there's a fatal error, this function will return false. Datagram sockets
   * generally won't report failures, because there is no state like a
   * "connection" to fail at this level, but may report failure for
   * unrecoverable system-level conditions; once a datagram socket fails, you
   * should assume it is no longer usable and should destroy it with
   * SDL_DestroyDatagramSocket().
   *
   * @param sock the datagram socket to send data through.
   * @returns true if data sent or queued for transmission, false on failure;
   *          call GetError() for details.
   *
   * @threadsafety You should not operate on the same socket from multiple
   *               threads at the same time without supplying a serialization
   *               mechanism. However, different threads may access different
   *               sockets at the same time without problems.
   *
   * @since This function is available since SDL_net 3.0.0.
   *
   * @sa SendDatagram
   * @sa DestroyDatagram
   */
  bool Receive(DatagramSocketRef sock);
};

/**
 * The data provided for new incoming packets from ReceiveDatagram().
 *
 * @since This datatype is available since SDL_net 3.0.0.
 *
 * @sa ReceiveDatagram
 * @sa DestroyDatagram
 *
 * @cat resource
 */
struct Datagram : DatagramBase
{
  using DatagramBase::DatagramBase;

  /**
   * Constructs from raw Datagram.
   *
   * @param resource a DatagramRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit Datagram(DatagramRaw resource) noexcept
    : DatagramBase(resource)
  {
  }

  /// Copy constructor
  constexpr Datagram(const Datagram& other) = delete;

  /// Move constructor
  constexpr Datagram(Datagram&& other) noexcept
    : Datagram(other.release())
  {
  }

  /**
   * Receive a new packet that a remote system sent to a datagram socket.
   *
   * Datagram sockets send packets of data. They either arrive as complete
   * packets or they don't arrive at all, so you'll never receive half a packet.
   *
   * This call never blocks; if no new data is available at the time of the
   * call, it returns true immediately. The caller can try again later.
   *
   * On a successful call to this function, it returns true, even if no new
   * packets are available, so you should check for a successful return and a
   * non-nullptr value in `*dgram` to decide if a new packet is available.
   *
   * You must pass received packets to DestroyDatagram when you are done with
   * them. If you want to save the sender's address past this time, it is safe
   * to call RefAddress() on the address and hold onto the pointer, so long as
   * you call UnrefAddress() on it when you are done with it.
   *
   * Since datagrams can arrive from any address or port on the network without
   * prior warning, this information is available in the Datagram object that is
   * provided by this function, and this is the only way to know who to reply
   * to. Even if you aren't acting as a "server," packets can still arrive at
   * your socket if someone sends one.
   *
   * If there's a fatal error, this function will return false. Datagram sockets
   * generally won't report failures, because there is no state like a
   * "connection" to fail at this level, but may report failure for
   * unrecoverable system-level conditions; once a datagram socket fails, you
   * should assume it is no longer usable and should destroy it with
   * DestroyDatagramSocket().
   *
   * @param sock the datagram socket to send data through.
   * @post a valid Datagram object if data sent or queued for transmission,
   *       nullptr on failure; call GetError() for details.
   *
   * @threadsafety You should not operate on the same socket from multiple
   *               threads at the same time without supplying a serialization
   *               mechanism. However, different threads may access different
   *               sockets at the same time without problems.
   *
   * @since This function is available since SDL_net 3.0.0.
   *
   * @sa SendDatagram
   * @sa DestroyDatagram
   */
  Datagram(DatagramSocketRef sock);

  /// Destructor
  ~Datagram() { NET_DestroyDatagram(get()); }

  /// Assignment operator.
  constexpr Datagram& operator=(Datagram&& other) noexcept
  {
    swap(*this, other);
    return *this;
  }

  /// Assignment operator.
  Datagram& operator=(const Datagram& other) = delete;
};

/**
 * Create and bind a new datagram socket.
 *
 * Datagram sockets follow different rules than stream sockets. They are not a
 * reliable stream of bytes but rather packets, they are not limited to talking
 * to a single other remote system, they do not maintain a single "connection"
 * that can be dropped, and they are more nimble about network failures at the
 * expense of being more complex to use. What makes sense for your app depends
 * entirely on what your app is trying to accomplish.
 *
 * Generally the idea of a datagram socket is that you send data one chunk
 * ("packet") at a time to any address you want, and it arrives whenever it gets
 * there, even if later packets get there first, and maybe it doesn't get there
 * at all, and you don't know when anything of this happens by default.
 *
 * This function creates a new datagram socket.
 *
 * This function does not block, and is not asynchronous, as the system can
 * decide immediately if it can create a socket or not. If this returns success,
 * you can immediately start talking to the network.
 *
 * You can specify an address to listen for connections on; this address must be
 * local to the system, and probably one returned by GetLocalAddresses(), but
 * almost always you just want to specify nullptr here, to listen on any address
 * available to the app.
 *
 * If you need to bind to a specific port (like a server), you should specify it
 * in the `port` argument; datagram servers should do this, so they can be
 * reached at a well-known port. If you only plan to initiate communications
 * (like a client), you should specify 0 and let the system pick an unused port.
 * Only one process can bind to a specific port at a time, so if you aren't
 * acting as a server, you should choose 0. Datagram sockets can send individual
 * packets to any port, so this just declares where data will arrive for your
 * socket.
 *
 * Datagram sockets don't employ any protocol (above the UDP level), so they can
 * talk to apps that aren't using SDL_net, but if you want to speak any protocol
 * beyond arbitrary packets of bytes, such as WebRTC, you'll have to implement
 * that yourself on top of the stream socket.
 *
 * Unlike BSD sockets or WinSock, you specify the port as a normal integer; you
 * do not have to byteswap it into "network order," as the library will handle
 * that for you.
 *
 * The caller may supply properties to customize behavior. This is optional, and
 * a value of zero for `props` will request defaults for all properties.
 *
 * These are the supported properties:
 *
 * - `prop.DatagramSocket.REUSEADDR_BOOLEAN`: true if the socket should be
 *   created even if a previous socket has recently used this address. For
 *   various reasons, networks prefer that there be some delay between apps
 *   reusing the same address, but this can be problematic when iterating
 *   quickly, for software development purposes or just restarting a crashed
 *   service. This property defaults to true (although it should be noted that,
 *   at the operating system level, this defaults to false!). If this property
 *   is false and the OS feels that not enough time has elapsed, socket creation
 *   will fail and this function will report an error.
 * - `prop.DatagramSocket.ALLOW_BROADCAST_BOOLEAN`: true if the socket should
 *   allow broadcasting. At the lower level, this will set `SO_BROADCAST` for
 *   IPv4 sockets, to allow sending to the subnet's broadcast address at the OS
 *   level. For IPv6, it'll join the all-nodes link-local multicast group,
 *   ff02::1, allowing sending and receiving there, more or less simulating the
 *   usual IPv4 broadcast semantics. Other protocols take similar approaches. If
 *   you do not intend to send or receive broadcast packets on this socket, set
 *   this property to false, or omit it, as it defaults to false. Note: IPv4
 *   will still be able to receive broadcast packets without this option, but
 *   IPv6 will not. Also see notes about sending to a broadcast address in
 *   SendDatagram().
 *
 * @param addr the local address to listen for connections on, or nullptr to
 *             listen on all available local addresses.
 * @param port the port on the local address to listen for connections on, or
 *             zero for the system to decide.
 * @param props properties of the new socket. Specify zero for defaults.
 * @returns a new DatagramSocket on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_net 3.0.0.
 *
 * @sa GetLocalAddresses
 * @sa DestroyDatagramSocket
 */
inline DatagramSocket CreateDatagramSocket(AddressRef addr,
                                           Uint16 port,
                                           PropertiesRef props)
{
  return DatagramSocket(addr, port, props);
}

inline DatagramSocket AddressBase::CreateDatagramSocket(Uint16 port,
                                                        PropertiesRef props)
{
  return DatagramSocket(get(), port, props);
}

inline DatagramSocket::DatagramSocket(AddressRef addr,
                                      Uint16 port,
                                      PropertiesRef props)
  : DatagramSocket(CheckError(NET_CreateDatagramSocket(addr, port, props)))
{
}

namespace prop::DatagramSocket {

constexpr auto REUSEADDR_BOOLEAN =
  NET_PROP_DATAGRAM_SOCKET_REUSEADDR_BOOLEAN; ///< Reuseaddr enabled.

constexpr auto ALLOW_BROADCAST_BOOLEAN =
  NET_PROP_DATAGRAM_SOCKET_ALLOW_BROADCAST_BOOLEAN; ///< Allow broadcast
                                                    ///< enabled.

} // namespace prop::DatagramSocket

/**
 * Send a new packet over a datagram socket to a remote system.
 *
 * Datagram sockets send packets of data. They either arrive as complete packets
 * or they don't arrive at all, as opposed to stream sockets, where individual
 * bytes might trickle in as they attempt to reliably deliver a stream of data.
 *
 * Datagram packets might arrive in a different order than you sent them, or
 * they may just be lost while travelling across the network. You have to plan
 * for this. As an added confusion, since SDL_net might send the same packet on
 * multiple interfaces, you might get duplicate packets, possibly from different
 * network addresses. You have to plan for this, too.
 *
 * You can send to any address and port on the network, but there has to be a
 * datagram socket waiting for the data on the other side for the packet not to
 * be lost.
 *
 * General wisdom is that you shouldn't send a packet larger than 1500 bytes
 * over the Internet, as bad routers might fragment or lose larger ones, but
 * this limit is not hardcoded into SDL_net and in good conditions you might be
 * able to send significantly more.
 *
 * This call never blocks; if it can't send the data immediately, the library
 * will queue it for later transmission. There is no query to see what is still
 * queued, as datagram transmission is unreliable, so you should never assume
 * anything about queued data.
 *
 * If there's a fatal error, this function will return false. Datagram sockets
 * generally won't report failures, because there is no state like a
 * "connection" to fail at this level, but may report failure for unrecoverable
 * system-level conditions; once a datagram socket fails, you should assume it
 * is no longer usable and should destroy it with SDL_DestroyDatagramSocket().
 *
 * Sending to a nullptr address is treated as a request to broadcast a packet.
 * Note that this will report failure immediately if the socket was not created
 * with broadcast permission. Broadcast packets are (more or less) sent to every
 * machine on the LAN, unconditionally.
 *
 * **WARNING**: It is possible to build a game where everyone is playing on the
 * same LAN, and every player is simply broadcasting packets. This is absolutely
 * the wrong thing to do, however. Broadcast packets go to every device on the
 * LAN, whether they want them or not. The game DOOM, in its heyday, was capable
 * of [bringing entire networks to their
 * knees](https://doomwiki.org/wiki/Doom_in_workplaces) , as many players on the
 * same network would all be broadcasting relentlessly.
 *
 * In practice, broadcasting sparingly can be useful for certain functionality:
 * a LAN-only client broadcasting a few packets to ask for available servers,
 * and running servers replying directly to that client without broadcasting at
 * all, is reasonable and safe. Once clients and servers have found each other,
 * they can communicate directly without any broadcasting at all. For
 * peer-to-peer games, once connection is established, it's better to either
 * send unique packets to each known player, or use a multicasting (which works
 * like broadcast, but only routes packets to devices that are explicitly
 * listening for it).
 *
 * With IPv6, which doesn't support broadcasts, broadcasting is faked with
 * multicast to the all-nodes link-local multicast group, ff02::1, either on a
 * specific interface or letting the OS choose the default. Other protocols
 * might fake broadcast operations in similar ways in the future.
 *
 * @param sock the datagram socket to send data through.
 * @param address the Address object address. May be nullptr to broadcast.
 * @param port the address port.
 * @param buf a pointer to the data to send as a single packet.
 * @param buflen the size of the data to send, in bytes.
 * @returns true if data sent or queued for transmission, false on failure; call
 *          GetError() for details.
 *
 * @threadsafety You should not operate on the same socket from multiple threads
 *               at the same time without supplying a serialization mechanism.
 *               However, different threads may access different sockets at the
 *               same time without problems.
 *
 * @since This function is available since SDL_net 3.0.0.
 *
 * @sa ReceiveDatagram
 */
inline bool SendDatagram(DatagramSocketRef sock,
                         AddressRef address,
                         Uint16 port,
                         const void* buf,
                         int buflen)
{
  return NET_SendDatagram(sock, address, port, buf, buflen);
}

inline bool DatagramSocketBase::SendDatagram(AddressRef address,
                                             Uint16 port,
                                             const void* buf,
                                             int buflen)
{
  return SDL::SendDatagram(get(), address, port, buf, buflen);
}

/**
 * Receive a new packet that a remote system sent to a datagram socket.
 *
 * Datagram sockets send packets of data. They either arrive as complete packets
 * or they don't arrive at all, so you'll never receive half a packet.
 *
 * This call never blocks; if no new data is available at the time of the call,
 * it returns true immediately. The caller can try again later.
 *
 * On a successful call to this function, it returns true, even if no new
 * packets are available, so you should check for a successful return and a
 * non-nullptr value in `*dgram` to decide if a new packet is available.
 *
 * You must pass received packets to DestroyDatagram when you are done with
 * them. If you want to save the sender's address past this time, it is safe to
 * call RefAddress() on the address and hold onto the pointer, so long as you
 * call UnrefAddress() on it when you are done with it.
 *
 * Since datagrams can arrive from any address or port on the network without
 * prior warning, this information is available in the Datagram object that is
 * provided by this function, and this is the only way to know who to reply to.
 * Even if you aren't acting as a "server," packets can still arrive at your
 * socket if someone sends one.
 *
 * If there's a fatal error, this function will return false. Datagram sockets
 * generally won't report failures, because there is no state like a
 * "connection" to fail at this level, but may report failure for unrecoverable
 * system-level conditions; once a datagram socket fails, you should assume it
 * is no longer usable and should destroy it with SDL_DestroyDatagramSocket().
 *
 * @param sock the datagram socket to send data through.
 * @param dgram a pointer to the datagram packet pointer.
 * @returns true if data sent or queued for transmission, false on failure; call
 *          GetError() for details.
 *
 * @threadsafety You should not operate on the same socket from multiple threads
 *               at the same time without supplying a serialization mechanism.
 *               However, different threads may access different sockets at the
 *               same time without problems.
 *
 * @since This function is available since SDL_net 3.0.0.
 *
 * @sa SendDatagram
 * @sa DestroyDatagram
 */
inline bool ReceiveDatagram(DatagramSocketRef sock, Datagram& dgram)
{
  return dgram.Receive(sock);
}

/**
 * Receive a new packet that a remote system sent to a datagram socket.
 *
 * Datagram sockets send packets of data. They either arrive as complete packets
 * or they don't arrive at all, so you'll never receive half a packet.
 *
 * This call never blocks; if no new data is available at the time of the call,
 * it returns true immediately. The caller can try again later.
 *
 * On a successful call to this function, it returns true, even if no new
 * packets are available, so you should check for a successful return and a
 * non-nullptr value in `*dgram` to decide if a new packet is available.
 *
 * You must pass received packets to DestroyDatagram when you are done with
 * them. If you want to save the sender's address past this time, it is safe to
 * call RefAddress() on the address and hold onto the pointer, so long as you
 * call UnrefAddress() on it when you are done with it.
 *
 * Since datagrams can arrive from any address or port on the network without
 * prior warning, this information is available in the Datagram object that is
 * provided by this function, and this is the only way to know who to reply to.
 * Even if you aren't acting as a "server," packets can still arrive at your
 * socket if someone sends one.
 *
 * If there's a fatal error, this function will return false. Datagram sockets
 * generally won't report failures, because there is no state like a
 * "connection" to fail at this level, but may report failure for unrecoverable
 * system-level conditions; once a datagram socket fails, you should assume it
 * is no longer usable and should destroy it with SDL_DestroyDatagramSocket().
 *
 * @param sock the datagram socket to send data through.
 * @return a valid Datagram object if data sent or queued for transmission,
 *         nullptr on failure; call GetError() for details.
 *
 * @threadsafety You should not operate on the same socket from multiple threads
 *               at the same time without supplying a serialization mechanism.
 *               However, different threads may access different sockets at the
 *               same time without problems.
 *
 * @since This function is available since SDL_net 3.0.0.
 *
 * @sa SendDatagram
 * @sa DestroyDatagram
 */
inline Datagram ReceiveDatagram(DatagramSocketRef sock)
{
  Datagram dgram;
  dgram.Receive(sock);
  return dgram;
}

inline bool DatagramSocketBase::ReceiveDatagram(Datagram& dgram)
{
  return dgram.Receive(*this);
}

inline Datagram DatagramSocketBase::ReceiveDatagram()
{
  return SDL::ReceiveDatagram(get());
}

inline Datagram::Datagram(DatagramSocketRef sock)
  : Datagram(sock.ReceiveDatagram())
{
}

inline bool DatagramBase::Receive(DatagramSocketRef sock)
{
  DatagramRaw dgram;
  if (!NET_ReceiveDatagram(sock, &dgram)) return false;
  *this = Datagram(dgram);
  return true;
}

/**
 * Dispose of a datagram packet previously received.
 *
 * You must pass packets received through ReceiveDatagram to this function when
 * you are done with them. This will free resources used by this packet and
 * unref its Address.
 *
 * If you want to save the sender's address from the packet past this time, it
 * is safe to call RefAddress() on the address and hold onto its pointer, so
 * long as you call UnrefAddress() on it when you are done with it.
 *
 * Once you call this function, the datagram pointer becomes invalid and should
 * not be used again by the app.
 *
 * @param dgram the datagram packet to destroy.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_net 3.0.0.
 */
inline void DestroyDatagram(DatagramRaw dgram) { NET_DestroyDatagram(dgram); }

inline void DatagramBase::Destroy() { DestroyDatagram(release()); }

/**
 * Enable simulated datagram socket failures.
 *
 * Often times, testing a networked app on your development machine--which might
 * have a wired connection to a fast, reliable network service--won't expose
 * bugs that happen when networks intermittently fail in the real world, when
 * the wifi is flakey and firewalls get in the way.
 *
 * This function allows you to tell the library to pretend that some percentage
 * of datagram socket data transmission will fail.
 *
 * The library will randomly lose packets (both incoming and outgoing) at an
 * average matching `percent_loss`. Setting this to zero (the default) will
 * disable the simulation. Setting to 100 means _everything_ fails
 * unconditionally and no further data will get through. At what percent the
 * system merely borders on unusable is left as an exercise to the app
 * developer.
 *
 * This is intended for debugging purposes, to simulate real-world conditions
 * that are various degrees of terrible. You probably should _not_ call this in
 * production code, where you'll likely see real failures anyhow.
 *
 * @param sock The socket to set a failure rate on.
 * @param percent_loss A number between 0 and 100. Higher means more failures.
 *                     Zero to disable.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_net 3.0.0.
 */
inline void SimulateDatagramPacketLoss(DatagramSocketRef sock, int percent_loss)
{
  NET_SimulateDatagramPacketLoss(sock, percent_loss);
}

inline void DatagramSocketBase::SimulateDatagramPacketLoss(int percent_loss)
{
  SDL::SimulateDatagramPacketLoss(get(), percent_loss);
}

/**
 * Dispose of a previously-created datagram socket.
 *
 * This will _abandon_ any data queued for sending that hasn't made it to the
 * socket. If you need this data to arrive, you should wait for confirmation
 * from the remote computer in some form that you devise yourself. Queued data
 * is not guaranteed to arrive even if the library made efforts to transmit it
 * here.
 *
 * Any data that has arrived from the remote end of the connection that hasn't
 * been read yet is lost.
 *
 * @param sock datagram socket to destroy.
 *
 * @threadsafety You should not operate on the same socket from multiple threads
 *               at the same time without supplying a serialization mechanism.
 *               However, different threads may access different sockets at the
 *               same time without problems.
 *
 * @since This function is available since SDL_net 3.0.0.
 *
 * @sa CreateDatagramSocket
 * @sa SendDatagram
 * @sa ReceiveDatagram
 */
inline void DestroyDatagramSocket(DatagramSocketRaw sock)
{
  NET_DestroyDatagramSocket(sock);
}

inline void DatagramSocketBase::Destroy() { DestroyDatagramSocket(release()); }

/**
 * Block on multiple sockets until at least one has data available.
 *
 * This is a complex function that most apps won't need, but it could be used to
 * implement a more efficient server or i/o thread in some cases.
 *
 * This allows you to give it a list of objects and wait for new input to become
 * available on any of them. The calling thread is put to sleep until such a
 * time.
 *
 * The following things can be specified in the `vsockets` array, cast to `void
 * *`:
 *
 * - Server (reports new input when a connection is ready to be accepted with
 *   AcceptClient())
 * - StreamSocket (reports new input when the remote end has sent more bytes of
 *   data to be read with ReadFromStreamSocket, or if the socket finished making
 *   its initial connection).
 * - DatagramSocket (reports new input when a new packet arrives that can be
 *   read with ReceiveDatagram).
 *
 * This function takes a timeout value, represented in milliseconds, of how long
 * to wait for resolution to complete. Specifying a timeout of -1 instructs the
 * library to wait indefinitely, and a timeout of 0 just checks the current
 * status and returns immediately.
 *
 * This returns the number of items that have new input, but it does not tell
 * you which ones; since access to them is non-blocking, you can just try to
 * read from each of them and see which are ready. If nothing is ready and the
 * timeout is reached, this returns zero. On error, this returns -1.
 *
 * @param vsockets an array of pointers to various objects that can be waited
 *                 on, each cast to a void pointer.
 * @param numsockets the number of pointers in the `vsockets` array.
 * @param timeout Number of milliseconds to wait for new input to become
 *                available. -1 to wait indefinitely, 0 to check once without
 *                waiting.
 * @returns the number of items that have new input, or -1 on error.
 *
 * @threadsafety You should not operate on the same socket from multiple threads
 *               at the same time without supplying a serialization mechanism.
 *               However, different threads may access different sockets at the
 *               same time without problems.
 *
 * @since This function is available since SDL_net 3.0.0.
 *
 * @sa CreateDatagramSocket
 * @sa SendDatagram
 * @sa ReceiveDatagram
 */
inline int WaitUntilInputAvailable(void** vsockets,
                                   int numsockets,
                                   Sint32 timeout)
{
  return NET_WaitUntilInputAvailable(vsockets, numsockets, timeout);
}

/// @}

} // namespace SDL

#endif /* SDL3PP_ENABLE_NET */

#endif /* SDL3PP_NET_H_ */
