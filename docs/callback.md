Callback
========

A callback is a invocable object passed to a function as a customization point.
Im addition of supporting the default SDL callbacks, whenever we have an 
`void *` userdata customization point, we use it wrap a std::function object,
with signature similar to original and with the "CB" suffix. See 
@ref callback-rules for more details.

This module and its submodules document the callback types that use this 
pattern and functions related to them. In general, SDL uses callbacks like this:

- [Calling immediately](#immediate-callback), before the function taking the
  callback returns;
- [Storing the callback internaly](#delayed-callback):
  - to call only once a [result completes](#result-callback);
  - to call multiple times, [listening to some condition](#listener-callback).

Immediate callback
==================

Immediate callback or synchronous callback is always invoked before the callee
function returns. Because of this, there is no need to be store/marshal data
as @ref delayed-callback does.

@todo list all immediate callbacks

Delayed callback
================

Delayed or Asynchronous callback is a callback that may be invoked after the 
callee returns. This might be when some event occurs or when a given result 
available.

Result callback
---------------

Result callback is invoked at single point and then discarded. This means that
after its called it safe to free any memory it is holding. The CallbackWrapper 
is perfect to safely wrap it and avoid dangling pointer. An example of such 
uses is RunOnMainThread().

@todo list all result callbacks


Listener callback
-----------------

Listener callback is invoked multiple times whenever the event it subscripted
happens. It's not safe to free memory after call, so a separate mechanism must
be provided.

When there is a single listener per program, like SetEventFilter(), you we can
use UniqueWrapper, which makes sure only a single one per type exists.

When there can be just a single callback listening per resource we can use 
KeyValueWrapper with a Resource pointer as key. An example of this technique is
in WindowBase::SetHitTest(HitTestFunction). We need to make sure we call 
KeyValueWrapper.erase() on the equivalent `*.Destroy()` function, again the 
example is WindowBase::Destroy().

When there is multiple callbacks simultaneously per program, we can use a 
combination of CallbackWrapper to wrap it and KeyValueWrapper to 
associate them with an id you can later use to remove it, as we use in both
AddEventWatch() and AddTimer().

@todo list all result callbacks
@todo Allow listener handle to be stored into a RAII object.
