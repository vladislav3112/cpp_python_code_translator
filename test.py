def factorial(N: int) -> int:
    res: int = 1
    for i in range(N):
        res*= i+1
    return res

def print_array(arr: list, N:int) -> None:
    for i in range(N):
        print(arr[i])

def main() ->None:
    letters: list = ["a", "b", "c"]

    for letter in letters:
        print(letter)
    if (8 > 12) and (3*5 == 15):
        print("OK")
    print_array(letters, 3)

    c: int = 0
    for i in range(12):
        c+=1
        print(i)

    for idx in range(4):
        print("Hello")
main()