import re

# Data class that store individual entries of data
class Data:
    def __init__(self, DOB:str, name:str, color:str, likes_peas:bool, dogs:str):
        self.DOB = DOB
        self.name = name
        self.color = color
        self.likes_peas = likes_peas
        self.dogs = dogs

    # method for output the object as string
    def __str__(self) -> str:
        fmt_str = ('-------------------------\n'
                    'Date of Birth: {}\n'
                    'Name: {}\n'
                    'Color: {}\n'
                    'Likes Peas: {}\n'
                    'Dogs or Cats: {}\n'
                    '-------------------------\n')
        return fmt_str.format(self.DOB, self.name, self.color, 'Yes' if self.likes_peas else 'No', self.dogs)

#check for validity of the fields, check for missing and extra fields
def repairFields(fields:list) -> list:
    # list of regular expression for verify each field entry [DOB, Name, Color, Likes Peas, Dogs or Cats]
    reg_str = [r'\d{4}-\d{2}-\d{2}', r'\w+', r'\w{2,}', r'[yn]', r'dogs|cats|both']
    for i in range(5):
        reg = re.compile(reg_str[i])
        if (not reg.match(fields[i])):
            fields.insert(i, "N/A")
    fields = fields[0:5]
    return fields

if __name__ == '__main__':
    database = {}
    
    # read database to input all the data
    with open('../test.data', 'r') as fo:
        for line in fo:
            line = line[:-1]    # remove newline character
            line = line.replace(' ', '')    #remove all white spaces
            #process the line if not start with # or empty
            if (not line.startswith('#') and not len(line) == 0):
                fields = line.split(',')
                fields = repairFields(fields)
                database[fields[1].lower()] = Data(fields[0], fields[1], fields[2], fields[3] == 'y', fields[4])
    
    # print the database in sorted by the key (name)
    keys = list(database.keys())
    keys.sort()
    for name in keys:
        print(database[name])

    # print the name that likes pea
    print('-------------------------')
    print("People that likes pea:")
    for name in keys:
        if (database[name].likes_peas):
            print(database[name].name)
    print('-------------------------\n')

    # Continuous allow user to loop us specific person
    while(1):
        try:
            # get the name from console
            name = input("Enter a name to look up: ").lower()
            # check is name exist
            if name in database:
                print(database[name])
            else:
                print("Unfortunately, the person you are looking up is not in the database")
        # termination of the script
        except KeyboardInterrupt:
            print()
            break