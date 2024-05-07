.PHONY:all
all:
	@cd Compile_Server;\
	make;\
	cd -;\
	cd OJ_Server;\
	make;\
	cd -;\

.PHONY:clean
clean:
	@cd Compile_Server;\
	make clean;\
	cd -;\
	cd OJ_Server;\
	make clean;\
	cd -;\

.PHONY:output
output:
	@mkdir -p output/Compile_Server;\
	mkdir -p output/OJ_Server;\
	cp -rf Compile_Server/compile_server output/Compile_Server/;\
	cp -rf Compile_Server/temp output/Compile_Server/;\
	cp -rf OJ_Server/oj_server output/OJ_Server/;\
	cp -rf OJ_Server/conf output/OJ_Server/;\
	cp -rf OJ_Server/lib output/OJ_Server/;\
	cp -rf OJ_Server/Questions output/OJ_Server/;\
	cp -rf OJ_Server/template_html output/OJ_Server/;\
	cp -rf OJ_Server/wwwroot output/OJ_Server/;


.PHONY:clean
clean:
	@cd Compile_Server;\
	make clean;\
	cd -;\
	cd OJ_Server;\
	make clean;\
	cd -;\
	rm -rf output;