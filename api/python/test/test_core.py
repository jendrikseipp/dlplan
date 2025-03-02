
from dlplan import VocabularyInfo, InstanceInfo, SyntacticElementFactory, State


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
    instance.add_static_atom("on_g", ["a", "b"])
    return instance


def test_vocabulary():
    """ Exhaustively test VocabularyInfo
    """
    vocabulary = generate_bw_vocabulary()

    p0, p1, p2, p3, p4 = vocabulary.get_predicates()
    assert p0.get_name() == "on"
    assert p0.get_index() == 0
    assert p0.get_arity() == 2
    assert p1.get_name() == "on_g"
    assert p1.get_index() == 1
    assert p1.get_arity() == 2
    assert p2.get_name() == "ontable"
    assert p2.get_index() == 2
    assert p2.get_arity() == 1
    assert p3.get_name() == "holding"
    assert p3.get_index() == 3
    assert p3.get_arity() == 1
    assert p4.get_name() == "clear"
    assert p4.get_index() == 4
    assert p4.get_arity() == 1
    assert vocabulary.exists_predicate_name("on")
    assert not vocabulary.exists_predicate_name("foo")


def test_instance():
    """ Exhaustively test InstanceInfo
    """
    vocabulary = generate_bw_vocabulary()
    p0, p1, p2, p3, p4 = vocabulary.get_predicates()
    instance = generate_bw_instance(vocabulary)

    a0, a1, a2, a3, a4, a5, a6, a7, a8 = instance.get_atoms()
    o0, o1 = instance.get_objects()

    assert a0.get_name() == "on(a,b)"
    assert a0.get_index() == 0
    assert a0.get_predicate().get_name() == "on"
    assert a0.get_predicate() == p0
    assert a0.get_objects() == [o0, o1]
    assert a1.get_name() == "on(b,a)"
    assert a1.get_index() == 1
    assert a1.get_predicate().get_name() == "on"
    assert a1.get_predicate() == p0
    assert a1.get_objects() == [o1, o0]
    assert a2.get_name() == "ontable(a)"
    assert a2.get_index() == 2
    assert a2.get_predicate().get_name() == "ontable"
    assert a2.get_predicate() == p2
    assert a2.get_objects() == [o0]

    assert o0.get_name() == "a"
    assert o0.get_index() == 0
    assert o1.get_name() == "b"
    assert o1.get_index() == 1


def test_factory():
    """ Test the construction, evaluation, and getters of Elements.
    """
    vocabulary = generate_bw_vocabulary()
    instance = generate_bw_instance(vocabulary)
    factory = SyntacticElementFactory(vocabulary)

    p0, p1, p2, p3, p4 = vocabulary.get_predicates()
    a0, a1, a2, a3, a4, a5, a6, a7, a8 = instance.get_atoms()
    state = State(instance, [a0, a3, a6])
    assert str(state) == "{on(a,b), ontable(b), clear(a), on_g(a,b)}"

    numerical = factory.parse_numerical("n_count(c_and(c_primitive(on_g,0),c_primitive(on,0)))")
    assert numerical.compute_complexity() == 4
    assert numerical.evaluate(state) == 1

    boolean = factory.parse_boolean("b_empty(c_and(c_primitive(on_g,0),c_primitive(on,0)))")
    assert boolean.compute_complexity() == 4
    assert boolean.evaluate(state) == 0

    numerical = factory.parse_numerical("n_count(c_projection(r_primitive(on_g,0,1), 1))")
    assert numerical.compute_complexity() == 3
    assert numerical.evaluate(state) == 1
