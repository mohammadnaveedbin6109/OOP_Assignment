# #list
# # lst=list()
# # print(lst)
# # lst = ['item1','item2','item3', 'item4', 'item5']
# # first_item, second_item, third_item, *rest = lst
# # print(first_item)     # item1
# # print(second_item)    # item2
# # print(third_item)     # item3
# # print(rest)           # ['item4', 'item5']
# # fruits = ['banana', 'orange', 'mango', 'lemon']
# # orange_and_mango = fruits[1:3]
# # print(orange_and_mango)
# # fruits = ['banana', 'orange', 'mango', 'lemon', 'kiwi', 'lime']
# # fruits = ['banana', 'orange', 'mango', 'lemon']
# # fruits.sort()
# # print(fruits) 

# #q1
# lst=list()
# print(lst)
# lst=[]
# print(lst)
# #q2
# mylst=['apple','banana','mango','stawberry','orange']
# #q3
# print(len(mylst))
# #q4
# print(mylst[0])
# print(mylst[int(5/2)])
# print(mylst[-1])
# #q5
# mixed_data_type=['muhammad','21','6.1','single','mianwali']
# it_companies='Facebook, Google, Microsoft, Apple, IBM, Oracle, Amazon'
# it_companies=it_companies.split(',')
# print(it_companies)
# print(mixed_data_type)
# #q8
# print(len(it_companies))
# #q9
# print(it_companies[0])
# print(it_companies[-1])
# it_companies[-2]='meta'
# print(it_companies)
# it_companies.append('IT company')
# print(it_companies)
# it_companies.insert(int(len(it_companies)/2),'IT company')
# print(it_companies)
# a,*b=it_companies
# a=a.upper()
# it_companies=[a]+b
# print(it_companies)
# it_companies='#'.join(it_companies)
# print(it_companies)
# exist='meta' in it_companies
# print(exist)
# it_companies='Facebook, Google, Microsoft, Apple, IBM, Oracle, Amazon'
# it_companies=it_companies.split(',')
# it_companies.sort()
# print(it_companies)
# it_companies.sort(reverse=True)
# print(it_companies)
# first_three=it_companies[0:3]
# last_three=it_companies[-3:]
# print(last_three)
# print(first_three)
# it_companies.pop(0)
# it_companies.pop()
# it_companies.clear()
# del it_companies
# front_end = ['HTML', 'CSS', 'JS', 'React', 'Redux']
# back_end = ['Node','Express', 'MongoDB']
# front_end=front_end+back_end
# full_stack=front_end.copy()
# print(full_stack)
# add='python','sql'
# full_stack.insert(4,add)
# print(full_stack)
#exercise 2
ages = [19, 22, 19, 24, 20, 25, 26, 24, 25, 24]
ages.sort()
minage=ages[0]
maxage=ages[len(ages)-1]
ages.append(minage)
ages.append(maxage)
print(ages)
ages.pop()
ages.pop()
print(ages)
median=ages[int(len(ages)/2)]
print(median)
