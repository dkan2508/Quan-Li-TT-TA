bin_dir_o = .\bin\o
bin_dir_exe = .\bin\exe
cjson_dir = ./lib/cJSON

# Rule to clone cJSON repo if not exists
$(cjson_dir)/cJSON.c:
	if not exist $(cjson_dir) ( \
		git clone https://github.com/DaveGamble/cJSON.git $(cjson_dir) \
	)

# Compile cJSON object
$(bin_dir_o)\cJSON.o: $(cjson_dir)/cJSON.c $(cjson_dir)/cJSON.h | $(cjson_dir)/cJSON.c
	gcc -c $(cjson_dir)/cJSON.c -o $(bin_dir_o)\cJSON.o

$(bin_dir_o)\general.o: general.c general.h
	gcc -c general.c -o $(bin_dir_o)\general.o

$(bin_dir_o)\linked_list.o: linked_list.c linked_list.h
	gcc -c linked_list.c -o $(bin_dir_o)\linked_list.o

$(bin_dir_o)\student.o: student.c student.h linked_list.h general.h $(cjson_dir)/cJSON.h
	gcc -c student.c -o $(bin_dir_o)\student.o

$(bin_dir_o)\teacher.o: teacher.c teacher.h linked_list.h general.h $(cjson_dir)/cJSON.h
	gcc -c teacher.c -o $(bin_dir_o)\teacher.o

$(bin_dir_o)\course.o: course.c course.h linked_list.h general.h $(cjson_dir)/cJSON.h
	gcc -c course.c -o $(bin_dir_o)\course.o

$(bin_dir_o)\class.o: class.c class.h linked_list.h general.h student.h teacher.h $(cjson_dir)/cJSON.h
	gcc -c class.c -o $(bin_dir_o)\class.o

$(bin_dir_o)\bill.o: bill.c bill.h linked_list.h general.h $(cjson_dir)/cJSON.h
	gcc -c bill.c -o $(bin_dir_o)\bill.o

$(bin_dir_o)\stat.o: stat.c stat.h general.h linked_list.h bill.h
	gcc -c stat.c -o $(bin_dir_o)\stat.o

$(bin_dir_o)\file.o: file.c file.h linked_list.h $(cjson_dir)/cJSON.h
	gcc -c file.c -o $(bin_dir_o)\file.o

$(bin_dir_o)\main.o: main.c general.h linked_list.h student.h teacher.h course.h class.h bill.h stat.h file.h
	gcc -c main.c -o $(bin_dir_o)\main.o

main: $(bin_dir_o)\main.o $(bin_dir_o)\general.o $(bin_dir_o)\linked_list.o $(bin_dir_o)\student.o $(bin_dir_o)\teacher.o $(bin_dir_o)\course.o $(bin_dir_o)\class.o $(bin_dir_o)\bill.o $(bin_dir_o)\stat.o $(bin_dir_o)\file.o $(bin_dir_o)\cJSON.o
	gcc -mconsole -o $(bin_dir_exe)\main.exe $(bin_dir_o)\general.o $(bin_dir_o)\linked_list.o $(bin_dir_o)\student.o $(bin_dir_o)\teacher.o $(bin_dir_o)\course.o $(bin_dir_o)\class.o $(bin_dir_o)\bill.o $(bin_dir_o)\main.o $(bin_dir_o)\stat.o $(bin_dir_o)\file.o $(bin_dir_o)\cJSON.o
