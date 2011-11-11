#!/usr/bin/env python

import threading
from time import sleep
from core.threadpool import ThreadPool, makeRequests


class ActionHandlerThread(threading.Thread):
    """
    The class for the thread that handles the LAP
    from a remote server
    """
    def __init__(self, plugins, options):
        self.plugins = plugins
        self.options = options
        threading.Thread.__init__(self)

    def run(self):
        pool = ThreadPool(10)
        while self.running:
            requests = makeRequests(some_callable, list_of_args)
            [pool.putRequest(req) for req in requests]
            pool.wait()

            sleep(1)
            print "ActionHandlerThread"



