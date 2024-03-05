for i in range(1, 51):
    with open("../mountdir/" + str(i), "w") as f:
        f.write("a" * 1024)