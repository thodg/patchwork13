<H2>Windows binary</H2>
Warning : this is an <b>beta version</b>, for testing only !<br>
<a href="http://sourceforge.net/projects/patchwork13" 
target=_blank>http://sourceforge.net/projects/patchwork13</a><br>
remember that patchwork13 is free
<br><br><br>
<H2>Source for Unix ,Mac & windows<H2>
<H3>Subversion access</H3>
Prerequisit: subversion<br>
go to the folder where you want the folder patchwork13 to be copied 
and type:
<PRE>
 # svn checkout https://svn.sourceforge.net/sourceforge.net/svnroot/patchwork13/
</PRE>
then read the file patchwork13/trunk/INSTALL for installation 
instruction
<!--
<H3>CVS access</H3>
You can download the latest sources in the CVS repository via anonymous login :
<PRE>
 # mkdir patchwork13
 # cd patchwork13

 # cvs -z3 -d:pserver:anonymous@cvs.sf.net:/cvsroot/patchwork13 checkout .
</PRE>
<br>
Or if you are a patchwork13 developer with sourceforge account :
<PRE>
 # export CVS_RSH="ssh"      (you may wish to put this in ~/.profile)

 # mkdir patchwork13
 # cd patchwork13

 # cvs -z3 -d:ext:LOGIN@cvs.sf.net:/cvsroot/patchwork13 checkout .
</PRE>
-->
