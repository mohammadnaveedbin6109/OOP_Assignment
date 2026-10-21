first_name = 'Asabeneh'
last_name = 'Yetayeh'
space = ' '
full_name = first_name  +  space + last_name
print(full_name)
# Strings only
first_name = 'Asabeneh'
last_name = 'Yetayeh'
language = 'Python'
formated_string = 'I am %s %s. I teach %s' %(first_name, last_name, language)
print(formated_string)
name='muhammad'
fname='he is %s'%(name)
print(fname)
a = 4
b = 3
print(f'{a} + {b} = {a +b}')
print(f'{a} - {b} = {a - b}')
print(f'{a} * {b} = {a * b}')
print(f'{a} / {b} = {a / b:.2f}')

language = 'Python'
a,b,c,d,e,f= language # unpacking sequence characters into variables
print(a) # P
print(b) # y
print(c) # t
print(d) # h
print(e) # o
print(f) # n

challenge = 'thirty days of python'
sub_string = 'da'
print(challenge.rindex(sub_string, 7)) # error
#q1
a='thirty'
b='days'
c='of'
d='python'
finalstring=a+b+c+d
print(finalstring)
#q2
a='coding'
b='for'
c='all'
finalstring=a+b+c
print(finalstring)
#q3
company='coding for all'
#q4
print(company)
#q5
print(len(company))
#q6
company=company.upper()
print(company)
#q7
company=company.lower()
print(company)
#q8
company='coding for all'
company=company.capitalize()
print(company)
company=company.title()
print(company)
company=company.swapcase()
print(company)
#q9
company=company.split()[0]
print(company)
#q10
company='coding for all'
finding='coding'
print(company.index(finding))
print(company.count(finding))
#q11
company=company.replace('coding','python')
print(company)
#q12
sentence='python for everyone'
change='all'
sentence=sentence.replace('everyone','all')
sentence='python for everyone'
sentence=sentence.split()
sentence[2]='all'
sentence=''.join(sentence)
print(sentence)
#q13
sentence='coding for all'
sentence=sentence.split()
print(sentence)
#q14
sentence='Facebook, Google, Microsoft, Apple, IBM, Oracle, Amazon'
sentence=sentence.split(',')
print(sentence)
#q15
sentence='coding for all'
print(sentence[0])
#q16
print(sentence[len(sentence)-1])
#q17
print(sentence[10])
#q18
sentence='Python For Everyone'
sentence=sentence.split()
a,b,c=sentence
a=a[0]
b=b[0]
c=c[0]
sentence=[a,b,c]
sentence=' '.join(sentence)
print(sentence)
#q19
sentence='coding for all'
sentence=sentence.split()
a,b,c=sentence
print(a)
a=a[0]
b=b[0]
c=c[0]
sentence=a+b+c
print(sentence)
#q20
sentence='coding for all'
print(sentence.index('c'))
#q21
print(sentence.index('f'))
#q22
sentence='Coding For All People'
print(sentence.rfind('l'))
#q23
sentence='You cannot end a sentence with because because because is a conjunction'
print(sentence.index('because'))
#24
print(sentence.rindex('because'))
#q25
sentence=sentence[31:54]
print(sentence)
#q26
sentence='Coding For All'
substring='Coding'
print(sentence.startswith(substring))
#29
print(sentence.endswith(substring))
#q30
sentence='   Coding For All     '
sentence=sentence.split()
a,b,c=sentence
sentence=[a,b,c]
sentence=''.join(sentence)
print(sentence)
sentence=['Django', 'Flask', 'Bottle', 'Pyramid', 'Falcon']
sentence='# '.join(sentence)
print(sentence)
print('I am enjoying this challenge.\nI just wonder what is next.')
sentence=['name','age','country','city']
sentence='\t'.join(sentence)
secsentence=['asabaneh','250','finland','helsenkei']
secsentence='\t'.join(secsentence)
print(f'{sentence}\n{secsentence}')
#q35
radius = 10
area = 3.14 * radius ** 2
print(f'The area of a circle with radius {radius} is {area} meters square.')

