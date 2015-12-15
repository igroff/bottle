### What?
It's a bottle: you put messages in it and other people will get them.
 
#### Really?
Bottle is a simply system by which subscribers can be notified of messages. The
system guarantees that a message will be sent to a subscriber at least once, in
practice only rarely will a message arrive at a subscriber more than once but
it is possible.
Bottle attempts to make it simple to register for notifications, while offering
a significant amount of flexability to the subscriber.
The system is comprised of an HTTP based api for sending messages, and a series
of N 'workers' that handle the routing of messages and notification of subscribers
when a message of interest arrives.


#### Definitions
* bottle - this system as a whole
* message - a JSON blob containing information about an event that has taken
  place. With the exception of the single required field (source [ see below])
  the JSON object that comprises the message is treated as opaque by bottle,
  you can put whatever you wish in it (just remember to have a source).
* message source - a path like structure indicating the origin of the message
  and used to to map messages to handlers. This is the only required portion of
  a bottle message.
* subscriber - ultimately messages are routed by making a determination to which
  HTTP URL the message will be POSTed.  An URL that receives a POSTed message is
  referred to as a subscriber.
* router - a system making decisions about what destinations will receive
  particular messages there are 1 to N routers active in a functioning bottle
  system at any time.  
* subscription repo - subscriptions to events are managed by a git repository containing
  a mapping of event information to subscribers. The subscription repo format
  is covered more fully below.
* subcription - a subscription is defined by a file containing a JSON object, describing
  at least, the URL to which the message is to be POSTed.


#### Design Choices

* shell scripts - This is new and in flux, making this out of shell scripts makes it
  super simple to adapt 'till we know better what works best.
* 'functions' - each shell script in ./bin is analogous to a function that one
  might write in another language.
* passing messages - this refers to 'message' as defined above in Definitions,
  when passing a message into a fuction (shell script) it will be passed in on
  stdin.
* All messages will be passed around in their full form, implying:
  * when sending a message it will be bare like it was created
      {"source": "/bottle/ping"}
  * when working with messages from the queues they will will look like whatever
    queueing system format is forced upon them.


#### Using Bottle to Send Messages

Following are the steps involved in using bottle to send messages to subscribers
the steps are abstracted somewhat from the underlying implementation to allow
a good enough understanding to discuss the usage of the system.

  1. Define a source - each bottle message must have a source, this source is
  a path like structure that serves as a unique identifier for a class of
  messages. If you're creating 'new' messages you probably want to create a
  source that's unique to your needs  If you're wanting to notify existing 
  subscribers of an event they've already delcared interest in, you can determine
  what source to use by inspecting the subscription repo.
  2. Determine what the message will contain - bottle only cares that your 
  message contain a 'source' property, aside from that the contents of the
  message are up to the 'creator' of the message. There's not even any reason
  to believe two messages from the same source need to contain the same data.
  3. Send the message - to send a message you can simply POST the JSON 
  representation of the message to the HTTP api endpoint ( e.g. 
  http://some.bottle.server/send ).

That's it, as far as using bottle is concerened all you need to do is send 
a message, it will handle the details of making sure the message gets to all
interested parties. 


#### Receiving Messages from bottle

Subscribing to messages sent by bottle involves creating a file in the 
subscription repository that bottle is using. All you need to do is create
a file containing a JSON object with a property 'notifyUrl' in the appropriate
location and you'll start receiving messages.

Subscribing to a message involves placing the file mentioned above at the
appropriate place in the subscription repository. The name of the file defining
the subscription is insiginificant, the path to it and the contents of it are
the important parts.

##### Sources and Paths
Within the subscription repository, the path structure that contains subscriptions
is used to match up message sources with subscribers. To subscribe to a message
given a particular source, you would find (or create) a matching path structure
in the subscription repository and place a subscription (file) there. An
example is in order:

For simplicity, all of our subscriptions will end in .json and anything without
an extension will refer to a directory. While git will not store empty directories
we'll not take that into account here (.  Our repository will exist in whichever
context you check it out, we'll be referring to the root as '/' this doesn't
imply anything about the true location of the subscription repository.

      / -
        |- bottle
        |  |- ping
        |     |- ping_me.json
        |
        |- databases
        |  |- sql
        |  |  |- server_name
        |  |     |- tell_me_when_something_on_server_name_happens.json
        |  |     |- schema_name
        |  |        |- table_name
        |  |           |- tell_me_when_table_name_changes.json       
        |  |           |- tell_me_when_key_33223_changes.json
        |  |- mysql
        |     | - tell_me_about_mysql_stuff.json
        |
        |- applications
        |  |- email
        |     |- tell_me_when_bob_is_sent_an_email.json
        |
        |- sausage
           |- on
              |- a_stick
        



* `send_message_to_router` - this is the recording of an event.  At this point something has taken place and we'd like to notify folks of that fact.  This event is generated by (most likely) some external system that would like to make known an action that it has taken. Most often this will be in the form of POSTing a message to this service.
* `get_message_from_routing_queue` - some number of processes will be monitoring the routing queue for messages and performing the following 3 steps
* `archive_message_from_routing_queue`
* `queue_notification_tasks_for_message`
* `delete_message_from_routing_queue`
* `get_task_from_queue` - some number of processes will be monitoring the task queue and performing the following 3 steps
* `perform_task`
* `archive_completed_task`
* `delete_message_from_task_queue`
