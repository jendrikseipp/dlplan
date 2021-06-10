
from dlplan import VocabularyInfo, InstanceInfo, SyntacticElementFactory, State, FeatureGenerator


def generate_bw_vocabulary():
    vocabulary = VocabularyInfo()
    vocabulary.add_predicate("on", 2)
    vocabulary.add_predicate("on_g", 2)
    vocabulary.add_predicate("ontable", 1)
    vocabulary.add_predicate("holding", 1)
    vocabulary.add_predicate("clear", 1)
    return vocabulary


def generate_bw_instance(vocabulary):
    instance = InstanceInfo(vocabulary)
    instance.add_atom("on", ["a", "b"])
    instance.add_atom("on", ["b", "a"])
    instance.add_atom("ontable", ["a"])
    instance.add_atom("ontable", ["b"])
    instance.add_atom("holding", ["a"])
    instance.add_atom("holding", ["b"])
    instance.add_atom("clear", ["a"])
    instance.add_atom("clear", ["b"])
    instance.add_atom("on_g", ["a", "b"])
    return instance


def test_generate_exhaustively():
    vocabulary = generate_bw_vocabulary()
    instance = generate_bw_instance(vocabulary)
    factory = SyntacticElementFactory(vocabulary)
    generator = FeatureGenerator(factory, 2, 180)
    atoms = instance.get_atoms()
    assert len(atoms) == 9
    s0 = State(instance, [atoms[0], atoms[3]])
    s1 = State(instance, [atoms[1], atoms[2]])
    s2 = State(instance, [atoms[2], atoms[3]])
    assert str(s0) == "{on(a,b), ontable(b)}"
    assert str(s1) == "{on(b,a), ontable(a)}"
    assert str(s2) == "{ontable(a), ontable(b)}"
    states = [s0, s1, s2]
    collection = generator.generate([])
