C++_SRCS += \
../Array.cc \
../Base.c++ \
../Base.cc \
../Files.cc \
../Graph.cc \
../Input.cc \
../Language.c++ \
../Language.cc \
../math.cc \
../SKBase.cc \
../Sounds.c++ \
../Sounds.cc \
../String.c++ \
../String.cc \
../Threads.cc \
../UI.cc \


C++_DEPS += \
./bin/Array.cc.d \
./bin/Base.c++.d \
./bin/Base.cc.d \
./bin/Files.cc.d \
./bin/Graph.cc.d \
./bin/Input.cc.d \
./bin/Language.c++.d \
./bin/Language.cc.d \
./bin/math.cc.d \
./bin/SKBase.cc.d \
./bin/Sounds.c++.d \
./bin/Sounds.cc.d \
./bin/String.c++.d \
./bin/String.cc.d \
./bin/Threads.cc.d \
./bin/UI.cc.d \


OBJS += \
./bin/Array.cc.o \
./bin/Base.c++.o \
./bin/Base.cc.o \
./bin/Files.cc.o \
./bin/Graph.cc.o \
./bin/Input.cc.o \
./bin/Language.c++.o \
./bin/Language.cc.o \
./bin/math.cc.o \
./bin/SKBase.cc.o \
./bin/Sounds.c++.o \
./bin/Sounds.cc.o \
./bin/String.c++.o \
./bin/String.cc.o \
./bin/Threads.cc.o \
./bin/UI.cc.o \



./bin/%.pyx.o: ../%.pyx subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cython Compiler'
	cython -o "./bin/nonezst/$<.c" "$<" --3str
	gcc  -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"./$(@:%.o=%.d)" -MT"$@" -o "$@" "./bin/nonezst/$<.c"  -L/usr/lib64  -ldl  -lm 
	@echo 'Finished building: $<'
	@echo ' '
./bin/%.pyxx.o: ../%.pyxx subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cython Compiler'
	cython --cplus -o "./bin/nonezst/$<.c++" "$<" --3str
	g++ -fmodules  -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"./$(@:%.o=%.d)" -MT"$@" -o "$@" "./bin/nonezst/$<.c++"  -L/usr/lib64  -ldl  -lm 
	@echo 'Finished building: $<'
	@echo ' '


./bin/%.s.o: ../%.s subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: (GNU as) Assembly Compiler'
	as -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

./bin/%.asm.o: ../%.asm subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: (GNU as) Assembly Compiler'
	as -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



./bin/%.go.o: ../%.go subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GO Compiler'
	go tool compile -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



./bin/%.class: ../%.java subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Javac java Compiler'
	javac -d "./" "$<"
	@echo 'Finished building: $<'
	@echo ' '



./bin/%.cpp.o: ../%.cpp subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: G++ C++ Compiler'
	g++ -std=c++2c  -fmodules -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"./$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
./bin/%.c++.o: ../%.c++ subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: G++ C++ Compiler'
	g++ -std=c++2c  -fmodules -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"./$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
./bin/%.cc.o: ../%.cc subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: G++ C++M Compiler'
	g++ -std=c++2c  -fmodules -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"./$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
./bin/%.cppm.o: ../%.cppm subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: G++ C++M Compiler'
	g++ -std=c++2c  -fmodules -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"./$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
./bin/%.c++m.o: ../%.c++m subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: G++ C++M Compiler'
	g++ -std=c++2c  -fmodules -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"./$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

./bin/%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc  -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"./$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean--2e-

clean--2e-:
	-$(RM) ./bin/*

.PHONY: clean--2e-
