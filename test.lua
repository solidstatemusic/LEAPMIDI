-- script.lua
-- Receives a table, returns the sum of its components.
io.write("The table the script received has:\n");
x = 0
for i = 1, #foo do
  print(i, foo[i])
  x = x + foo[i]
end
x = 0
io.write("Before add, x was...\n");
print(x);
io.write("Now it's...\n");
x = addone(x);
print(x);
io.write("Returning data back to C\n");
return x
