O::Expected
===========

Overview
--------

.. doxygenclass:: O::Expected
	:members:
	:protected-members:
	:private-members:
	:undoc-members:


Basic Usage
-----------
Construct an ``Expected`` from a value:

.. code-block:: cpp

	O::Expected<int, std::string> good = O::Expected<int, std::string>::Make_Value(42);

Check for success:

.. code-block:: cpp

	if (good)
	{
		int v = good.Value();
	}

Construct from an error:

.. code-block:: cpp

	auto bad = O::Expected<int, std::string>::Make_Error("something went wrong");

Inspect an error:

.. code-block:: cpp

	if (!bad)
	{
		std::string msg = bad.Error();
	}

Moving values:

``Value()`` and ``Error()`` are available in ``&`` and ``&&`` overloads, so you
can efficiently move the stored object:

.. code-block:: cpp

	int v = std::move(good).Value();
