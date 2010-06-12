#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
import os.path

from setuptools import setup

setup(
    name = 'TracSourceWikiInclude',
    version = '0.1',
    packages = ['sourcewikiinclude'],

    author = 'Alexander Kasper',
    author_email = 'alexander.kasper@rub.de',
    description = 'includes a wikipage from the repos.',
    long_description = open(os.path.join(os.path.dirname(__file__), 'README')).read(),
    license = 'BSD',
    keywords = 'trac 0.11 plugin wiki include page macro',
    url = 'https://www.das-labor.org/',
    classifiers = [
        'Framework :: Trac',
    ],
    
    install_requires = ['Trac'],

    entry_points = {
        'trac.plugins': [
            'sourcewikiinclude.macros = sourcewikiinclude.macros',
        ]
    }
)
