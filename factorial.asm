push 5
call 1
# foo begin
:1
pop [0]
push 1
push 1
pop [1]

# while begin
:2

# res *= val
push [1]
push [0]
mul
pop [1]

# val -= 1
push [0]
push -1
add
pop [0]

# if !(val > 1) ---> break
push [0]
ja 2

push [1]
ret
out
hlt
