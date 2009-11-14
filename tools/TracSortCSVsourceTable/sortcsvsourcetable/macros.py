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

__all__ = ['SortCSVsourceTableMacro']

class SortCSVsourceTableMacro(WikiMacroBase):
    """
    A macro to create a sortable table out of a file in the source repository.
    """
    
    implements(IPermissionRequestor)
    
    # Default output formats for sources that need them
    default_formats = {
        'wiki': 'text/x-trac-wiki',
    }
    
    # IWikiMacroProvider methods
    def render_macro(self, req, name, content):
        args = [x.strip() for x in content.split(',')]
        if len(args) == 1:
            args.append(None)
        elif len(args) != 2:
            return system_message('Invalid arguments "%s"'%content)
            
        # Pull out the arguments
        source, theader = args
        try:
            source_format, source_obj = source.split(':', 1)
        except ValueError: # If no : is present, assume its a wiki page
            source_format, source_obj = 'source', source
            
        # Apply a default format if needed
        isTheader, theader_content = theader.split("=",1)
        
        if dest_format is None:
            try:
                dest_format = self.default_formats[source_format]
            except KeyError:
                pass
        
        if source_format == 'source':
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
        else:
            return system_message('Unsupported include source %s'%source)
            
        # If we have a preview format, use it
        if dest_format:
            out = Mimeview(self.env).render(ctxt, dest_format, out)
        
        # Escape if needed
#        if not self.config.getbool('wiki', 'render_unsafe_content', False):
#            try:
#                out = HTMLParser(StringIO(out)).parse() | HTMLSanitizer()
#            except ParseError:
#                out = escape(out)
        reader = str(out).split("||")
        need_header=1;
        foo=''
        foo+='<table class="sortable"  style="border:1px solid #000;">'
        foo+='<tr>'
        if isTheader == "header":
            custom_theader = theader_content.split(",")
            for theader_cell in custim_theader:
                foo+='<th style="border:1px solid #000;">'+str(theader_cell)+'</th>'

        for row in str(out).splitlines():
            foo += "</tr><tr>\n"
            for cell in row.split("||"):
                if cell.startswith("|"):
                    foo+='<td align="right" style="border:1px solid #000;">'+cell.lstrip("|")+"</td>"
                else:
                    foo+='<td align="right" style="border:1px solid #000;">'+cell+"</td>"
#            foo +=str(row)
#            for cell in row:
#                foo += cell
#                foo += "-"
        foo +="</tr></table>"
        
        return foo            
    # IPermissionRequestor methods
    def get_permission_actions(self):
        yield 'INCLUDE_URL'
            
        
        
