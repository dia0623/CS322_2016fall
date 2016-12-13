import ply.lex as lex
import ply.yacc as yacc

class AST :
    def __init__(self, value) :
        self.value = value
        self.parent = None
        self.leftChild = None
        self.rightChild = None

class eNFA :
    def __init__(self) :
        self.states = None
        self.symbols = None
        self.func = None
        self.ini = None
        self.fin = None

    def setNFA(self, states, symbols, func, ini, fin) :
        self.states = states
        self.symbols = symbols
        self.func = func
        self.ini = ini
        self.fin = fin

    def getNFA(self) :
        return (self.states, self.symbols, self.func, self.ini, self.fin)

#tokens = ('SYMBOL', 'OR', 'PLUS', 'STAR', 'LPARSEN', 'RPARSEN')
tokens = ('SYMBOL', 'OR', 'STAR', 'LPARSEN', 'RPARSEN')

t_SYMBOL    = r'[a-zA-Z0-9]'
#t_OR        = r'\|'
#t_PLUS      = r'\+'
t_OR        = r'\+'
t_STAR      = r'\*'
t_LPARSEN   = r'\('
t_RPARSEN   = r'\)'
t_ignore    = ' \t'

def t_error(t) :
    print "Illegal character '%s'" %t.value[0]
    t.lexer.skip(1)

#precedence = (('left', 'OR'),
#              ('left', 'PLUS', 'STAR'))
precedence = (('left', 'OR'),
              ('left', 'STAR'))

def p_expression_symbol(t) :
    'expression : SYMBOL'
    t[0] = t[1]

def p_expression_or(t) :
    'expression : expression OR expression'
#    t[0] = ('|', t[1], t[3])
    t[0] = ('+', t[1], t[3])

def p_expression_group(t) :
    'expression : LPARSEN expression RPARSEN'
    t[0] = t[2]

def p_expression_epsilon(t) :
    'expression : LPARSEN RPARSEN'
    t[0] = 'E'

#def p_expression_plus(t) :
#    'expression : expression PLUS expression'
#    t[0] = ('+', t[1])

def p_expression_concat(t) :
    'expression : expression expression'
    t[0] = ('.', t[1], t[2])

def p_expression_star(t) :
    'expression : expression STAR'
    t[0] = ('*', t[1])

def p_error(t) :
    print "Syntax error at '%s'" % t.value

def makeAST(parsed) :
    if (parsed == None) :
        return None
        
    if len(parsed) == 1 :
        root = AST(parsed)
    else :
        root = AST(parsed[0])

        leftChild = makeAST(parsed[1])
        leftChild.parent = root
        root.leftChild = leftChild
            
        if len(parsed) == 3 :
            rightChild = makeAST(parsed[2])
            rightChild.parent = root
            root.rightChild = rightChild

    return root

state_num = 0

def eNFA_SYMBOL(value) :
    global state_num
    
    newNFA = eNFA()
    newNFA.setNFA([state_num, state_num + 1],
                  [value],
                  [[(state_num, value), state_num + 1]],
                  state_num,
                  [state_num + 1])
    state_num = state_num + 2
    return newNFA


def merge(list1, list2) :
    temp1 = [var for var in list1]
    temp2 = [var for var in list2 if var not in temp1]

    result = temp1 + temp2
    result.sort()

    return result

def eNFA_OR(left, right) :
    global state_num

    (left_states, left_symbols, left_func, left_ini, left_fin) = left.getNFA()
    (right_states, right_symbols, right_func, right_ini, right_fin) = right.getNFA()

    left_states.extend(right_states)
    left_states.append(state_num)
    left_states.append(state_num + 1)
    #state_num = state_num + 2

    left_symbols = merge(left_symbols, right_symbols)

    left_func.extend(right.func)
    left_func.append([(state_num, 'E'), left_ini])
    left_func.append([(state_num, 'E'), right_ini])

    for fin in left_fin :
        left_func.append([(fin, 'E'), state_num + 1])
    for fin in right_fin :
        left_func.append([(fin, 'E'), state_num + 1])

    left_ini = state_num
    left_fin = [state_num + 1]
    state_num = state_num + 2

    newNFA = eNFA()
    newNFA.setNFA(left_states, left_symbols, left_func, left_ini, left_fin)

    return newNFA

def eNFA_CONCAT(left, right) :
    (left_states, left_symbols, left_func, left_ini, left_fin) = left.getNFA()
    (right_states, right_symbols, right_func, right_ini, right_fin) = right.getNFA()

    left_states.extend(right_states)

    left_symbols = merge(left_symbols, right_symbols)

    left_func.extend(right_func)

    for fin in left_fin :
        left_func.append([(fin, 'E'), right_ini])

    left_fin = right_fin

    newNFA = eNFA()
    newNFA.setNFA(left_states, left_symbols, left_func, left_ini, left_fin)

    return newNFA

def eNFA_STAR(nfa) :
    global state_num
    #print type(nfa)
    (states, symbols, func, ini, fin) = nfa.getNFA()

    states.append(state_num)
    states.append(state_num + 1)

    func.append([(state_num, 'E'), state_num + 1])
    func.append([(state_num, 'E'), ini])
    
    for final in fin :
        func.append([(final, 'E'), ini])
        func.append([(final, 'E'), state_num + 1])

    ini = state_num
    fin = [state_num + 1]

    state_num = state_num + 2

    newNFA = eNFA()
    newNFA.setNFA(states, symbols, func, ini, fin)

    return newNFA

def eNFA_PLUS(nfa) :
    global state_num
    (states, symbols, func, ini, fin) = nfa.getNFA()

    states.append(state_num)
    states.append(state_num + 1)

    func.append([(state_num, 'E'), ini])

    for final in fin :
        func.append([(final, 'E'), ini])
        func.append([(final, 'E'), state_num + 1])

    state_num = state_num + 2

    newNFA = eNFA()
    newNFA.setNFA(states, symbols, func, ini, fin)

    return newNFA    

def AST2NFA(ast) :
    if ast.value == '+' :
        l = AST2NFA(ast.leftChild)
        r = AST2NFA(ast.rightChild)
        return eNFA_OR(l, r)
    elif ast.value == '.' :
        l = AST2NFA(ast.leftChild)
        r = AST2NFA(ast.rightChild)
        return eNFA_CONCAT(l, r)
    elif ast.value == '*' :
        l = AST2NFA(ast.leftChild)
        return eNFA_STAR(l)
    else :
        return eNFA_SYMBOL(ast.value)

lex.lex()
yacc.yacc()

re = open("re.txt", "r")
reg_exp = re.readline()
parsed = yacc.parse(reg_exp)

myAST = makeAST(parsed)
myeNFA = AST2NFA(myAST)

out = open("e-nfa.txt", "w")

(states, symbols, func, ini, fin) = myeNFA.getNFA()

out.write("State\n")
cnt = 0
for state in states :
    if cnt == 0 :
        out.write("%d" %state)
    else :
        out.write(",%d" %state)

    cnt = cnt + 1
out.write("\n")

out.write("Input symbol\n")
cnt = 0
for symbol in symbols :
    if symbol != 'E' :
        if cnt == 0 :
            out.write("%s" %symbol)
        else :
            out.write(",%s" %symbol)
    cnt = cnt + 1
out.write("\n")

out.write("State transition function\n")
for trans in func :
    (state1, sym) = trans[0]
    out.write("%d,%s,%d\n" % (state1, sym, trans[1]))

out.write("Initial state\n")
out.write("%d\n" %ini)

out.write("Final state\n")
cnt = 0
for final in fin :
    if cnt == 0 :
        out.write("%d" %final)
    else :
        out.write(",%d"  %final)
    cnt = cnt + 1

re.close()
out.close()
