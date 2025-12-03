Setup
=====

project
-------
	project require C++20 as minimum requirement
	and cmake 3.15

	project as only been tested using MSVC compiler 

documentation
-------------
	generating the documentation require **python 3.10** with Sphinx , breathe and the read the doc them installed 

	.. code-block:: 

		pip install sphinx
		pip install breathe
		pip install sphinx_rtd_theme
		pip install sphinx-cmake

	you will also need the latest Doxygen vesion installed

	since the documentation use graphics to be generated you will also generate graphviz from <https://graphviz.org/>

	automatic graphical theming for the dot files also need the pydot 

	.. code-block::

		pip install pydot

CMakeLists
----------

.. code-block:: cmake

	include(FetchContent)

	FetchContent_Declare(
		OGeoFlow
		GIT_REPOSITORY https://github.com/leocity207/OUtils.git
		UPDATE_DISCONNECTED ON
	)

	FetchContent_MakeAvailable(OUtils)