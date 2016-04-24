


OBJFILES =\
disk_folder_manager.o\
disk_img_manager.o\
file_dir_iterator.o\
disk_file_manager.o

all: disk_img_app

disk_img_app: disk_img_app.c $(OBJFILES)
	gcc disk_img_app.c -o $@ $(OBJFILES)
	
clean:
	rm disk_img_app $(OBJFILES)
	
run:
	reset
	make disk_img_app
	./disk_img_app imgs/450_3.img