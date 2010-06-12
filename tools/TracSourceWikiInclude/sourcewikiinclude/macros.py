# TracIncludeMacro macros
import urllib2
import csv
from StringIO import StringIO

from trac.core import *
from trac.wiki.macros import WikiMacroBase
from trac.wiki.formatter import system_message
from trac.wiki.model import WikiPage
from trac.mimeview.api import Mimeview, get_mimetype, Context
from trac.perm import IPermissionRequestor
from genshi.core import escape
from genshi.input import HTMLParser, ParseError
from genshi.filters.html import HTMLSanitizer

__all__ = ['SourceWikiIncludeMacro']

class SourceWikiIncludeMacro(WikiMacroBase):
    """
    A macro to include a wikipage form a file in the source repository.
    """
    
    implements(IPermissionRequestor)
    
    # Default output formats for sources that need them
    default_formats = {
        'source': '',
    }
    
    # IWikiMacroProvider methods
    def render_macro(self, req, name, content):
            
        # Pull out the arguments
        source_obj  = str(content)
            
        dest_format = "text/x-trac-wiki"
        
        if not req.perm.has_permission('FILE_VIEW'):
            return ''
        repo = self.env.get_repository(req.authname)
        node = repo.get_node(source_obj)
        out = node.get_content().read()
        if dest_format is None:
            dest_format = node.content_type or get_mimetype(source_obj, out)
        ctxt = Context.from_request(req, 'source', source_obj)
        # RFE: Add ticket: and comment: sources. <NPK>
        # RFE: Add attachment: source. <NPK>
            
        # If we have a preview format, use it
        
        # Escape if needed
#        if not self.config.getbool('wiki', 'render_unsafe_content', False):
#            try:
#                out = HTMLParser(StringIO(out)).parse() | HTMLSanitizer()
#            except ParseError:
#                out = escape(out)
#        reader = str(out).split("||")
        need_header=1;
        foo=(str(Mimeview(self.env).render(None, "text/x-trac-wiki", str(out))))
        
        
        return foo
    # IPermissionRequestor methods
    def get_permission_actions(self):
        yield 'INCLUDE_URL'
            
        
        
