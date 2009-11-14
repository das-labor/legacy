#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
import os.path

from setuptools import setup

setup(
    name = 'TracSortCSVSourceTable',
    version = '0.1',
    packages = ['sortcsvsourcetable'],

    author = 'Alexander Kasper',
    author_email = 'alexander.kasper@rub.de',
    description = 'create a sortable table out off a file in the repository.',
    long_description = open(os.path.join(os.path.dirname(__file__), 'README')).read(),
    license = 'BSD',
    keywords = 'trac 0.11 plugin wiki sort csv include table macro',
    url = 'http://trac-hacks.org/wiki/IncludeMacro',
    classifiers = [
        'Framework :: Trac',
    ],
    
    install_requires = ['Trac'],

    entry_points = {
        'trac.plugins': [
            'sortcsvsourcetable.macros = sortcsvsourcetable.macros',
        ]
    }
)
