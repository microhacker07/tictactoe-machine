name = 'tictactoe'
print('Compiling ' + name)

env = Environment()
env.Append(LIBS = [
	'sfml-window',
	'sfml-system',
	'sfml-graphics'
]);

VariantDir("build", "source", duplicate = 0)

program = env.Program(name, Glob('build/*.cpp'))
env.Install("..", program)
