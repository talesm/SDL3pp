Callback
========

A callback is a invocable object passed to a function as a customization point.
Im addition of supporting the default SDL callbacks, whenever we have an
`void *` userdata customization point, we use it wrap a std::function object,
with signature similar to original and with the "CB" suffix. See @ref
callback-rules for more details.

This module and its submodules document the callback types that use this pattern
and functions related to them. In general, SDL uses callbacks like this:

- [Calling immediately](#immediate-callback), before the function taking the
  callback returns;
- [Storing the callback internaly](#delayed-callback):
  - to call only once a [result completes](#result-callback);
  - to call multiple times, [listening to some condition](#listener-callback).

Immediate callback
==================

Immediate callback or synchronous callback is always invoked before the callee
function returns. Because of this, there is no need to be store/marshal data as
@ref delayed-callback does.

@todo list all immediate callbacks

Delayed callback
================

Delayed or Asynchronous callback is a callback that may be invoked after the
callee returns. This might be when some event occurs or when a given result
available.

Result callback
---------------

Result callback is invoked only once at single point and then discarded. This
means that after its called it safe to free any memory it is holding. The
CallbackWrapper is perfect to safely wrap it and avoid dangling pointer. An
example of such uses is RunOnMainThread().

@todo list all result callbacks

Listener callback
-----------------

Listener callback is invoked multiple times whenever the event it subscripted
happens. It's not safe to free memory after call, so a separate mechanism must
be provided.

When there is a single listener per program, like SetEventFilter(), we can use a
full std::function and store it statically.

When there are multiple listeners for a given type, it is safer to use the
LightweightCallbackT or its shorthands MakeFrontCallback and MakeBackCallback.

@todo list all result callbacks @todo Allow listener handle to be stored into a
RAII object.
