n = int(input())
sqs = list(map(int, input().split()))
new_sqs = [sqs[0]]

for i in range(1, n):
    new_sqs.append(new_sqs[i-1] + sqs[i])
print(*new_sqs)