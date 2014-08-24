### What?
No idea. K, idea, somewhat incoherent.


Given a directory structure, messages are processed, using the directory structure
as a map for what messages will be delivered where


#### Definitions
* message - a JSON blob containing information about an event that has taken place
* message source - a path like structure indicating the origin of the message
  and used to to map messages to handlers.
* destination - ultimately messages are routed by making a determination to which
  HTTP URL the message will be posted.  An URL that receives a POSTed message is
  a destination.


