# This file is called before any tests are run.  Its purpose is to add the 
# parent directory to the system path, so that the physis.pyd library can
# be imported by each test module.

import os
import sys

parent_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
if parent_dir not in sys.path:
    sys.path.insert(0, parent_dir)