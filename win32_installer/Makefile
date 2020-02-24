
NSH =     pw13.i.dll.nsh         pw13.u.dll.nsh	\
      pw13_std.i.dll.nsh     pw13_std.u.dll.nsh	\
      pw13_sdl.i.dll.nsh     pw13_sdl.u.dll.nsh	\
    pw13_sound.i.dll.nsh   pw13_sound.u.dll.nsh	\
       cluster.i.dll.nsh      cluster.u.dll.nsh	\
      pw13_gtk.i.dll.nsh     pw13_gtk.u.dll.nsh

BUILD_DIR=$(shell pwd)

default: patchwork13.nsi $(NSH) .libs
	makensis $<

.PHONY: $(NSH)

pw13.i.dll.nsh:
	ls ../pw13/pw13/.libs/*.dll | \
	awk '{print "  File \""$$1"\""}' | \
	sed 'y+/+\\+' > $@

pw13.u.dll.nsh:
	cd ../pw13/pw13/.libs/; \
	ls *.dll | \
	awk '{print "  Delete \"$$INSTDIR/bin/"$$1"\""}' | \
	sed 'y+/+\\+' > $(BUILD_DIR)/$@

pw13_std.i.dll.nsh: 
	./find_pw13_libs install ../pw13_std std > $@

pw13_std.u.dll.nsh: 
	./find_pw13_libs uninstall ../pw13_std std > $@

pw13_sdl.i.dll.nsh:
	./find_pw13_libs install ../pw13_sdl sdl > $@

pw13_sdl.u.dll.nsh:
	./find_pw13_libs uninstall ../pw13_sdl sdl > $@

pw13_sound.i.dll.nsh:
	./find_pw13_libs install ../pw13_sound sound > $@

pw13_sound.u.dll.nsh:
	./find_pw13_libs uninstall ../pw13_sound sound > $@

cluster.i.dll.nsh:
	ls ../cluster/*/.libs/*.dll | \
	awk '{print "  File \""$$1"\""}' | \
	sed 'y+/+\\+' > $@

cluster.u.dll.nsh:
	cd ../cluster/*/.libs/; \
	ls *.dll | \
	awk '{print "  Delete \"$$INSTDIR/bin/"$$1"\""}' | \
	sed 'y+/+\\+' > $(BUILD_DIR)/$@

pw13_gtk.i.dll.nsh:
	./find_pw13_libs install ../pw13_gtk gtk > $@

pw13_gtk.u.dll.nsh:
	./find_pw13_libs uninstall ../pw13_gtk gtk > $@

.libs:
	mkdir libs 2>/dev/null; true
	cp 	`which SDL.dll` `which libglade-2.0-0.dll` \
		`which libxml2-2.dll` `which libxml2.dll` \
		`which pthreadGC2.dll` \
		libs

clean:
	rm -f ${NSH}; true
	rm -f *.exe; true
	rm -rf libs; true
