#ifndef DLPLAN_SRC_CORE_ELEMENTS_ELEMENT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ELEMENT_H_

#include <iostream>
#include <sstream>
#include <mutex>
#include <cassert>
#include <memory>

#include "types.h"

#include "../../../include/dlplan/utils/cache.h"
#include "../../../include/dlplan/core.h"


using namespace std::string_literals;

namespace dlplan::core::element {
class DenotationsCaches;

template<typename T>
class Element : public utils::Cachable {
protected:
    /**
     * Index can be used for internal caching.
     */
    int m_index;

public:
    // Elements are not copieable because they must live in the cache.
    explicit Element(const VocabularyInfo&) : m_index(-1) { }
    Element(const Element& other) = delete;
    Element& operator=(const Element& other) = delete;
    Element(Element&& other) = delete;
    Element& operator=(Element&& other) = delete;
    virtual ~Element() = default;

    virtual T evaluate(const State& state) const = 0;
    virtual const std::vector<const T*>* evaluate(const States& states, DenotationsCaches& caches) const = 0;

    virtual int compute_complexity() const = 0;

    virtual void compute_repr(std::stringstream& out) const = 0;

    void set_index(int index) {
        m_index = index;
    }

    virtual std::string compute_repr() const {
        std::stringstream ss;
        compute_repr(ss);
        return ss.str();
    }

    int get_index() const {
        return m_index;
    }
};


template<typename CONST_DENOT>
struct DenotationEqual {
public:
    bool operator()(const CONST_DENOT& l, const CONST_DENOT& r) const {
        return l->get_blocks() == r->get_blocks();
    }
};


template<typename CONST_DENOT>
struct DenotationHasher {
public:
    std::size_t operator()(const CONST_DENOT& denotation) const {
        return denotation->compute_hash();
    }
};


/**
 * Caches single denotations of type T.
 * We store everything as pointers for faster resize
 * and no invalidation on resize.
 */
template<typename T>
class DenotationCache {
private:
    using CONST_DENOT = const T*;

    std::unordered_set<CONST_DENOT, DenotationHasher<CONST_DENOT>, DenotationEqual<CONST_DENOT>> m_storage;

    // optional mapping from (instance, state, element) -> CONST_DENOT
    std::vector<std::vector<std::unordered_map<int, CONST_DENOT>>> m_mapping;

    int m_num_objects;
public:
    explicit DenotationCache(int num_objects) : m_num_objects(num_objects) { }
    DenotationCache(const DenotationCache& other) = delete;
    DenotationCache& operator=(const DenotationCache& other) = delete;
    DenotationCache(DenotationCache&& other) = default;
    DenotationCache& operator=(DenotationCache&&) = default;
    ~DenotationCache() {
        for (auto denot : m_storage) {
            delete denot;
        }
    }

    T* get_new_denotation() const {
        return new T(m_num_objects);
    }

    /**
     * Uniquely inserts a denotation and returns a reference to it.
     * Second alternative also creates mapping (instance, state, element) -> denotation*
     */
    CONST_DENOT insert(CONST_DENOT denotation) {
        return *m_storage.insert(denotation).first;
    }
    CONST_DENOT insert(CONST_DENOT denotation, int instance_index, int state_index, int element_index) {
        auto result = insert(denotation);
        if (instance_index >= m_mapping.size())
            m_mapping.resize(instance_index + 1);
        if (state_index >= m_mapping[state_index].size())
            m_mapping[instance_index].resize(state_index + 1);
        m_mapping[instance_index][state_index].emplace(element_index, denotation);
    }

    /**
     * Returns a ptr to the denotation if it exists and otherwise creates
     * an entry in the mapping that maps to nullptr.
     */
    CONST_DENOT find(int instance_index, int state_index, int element_index) const {
        if (instance_index >= m_mapping.size())
            return nullptr;
        if (state_index >= m_mapping[state_index].size())
            return nullptr;
        auto result = m_mapping[instance_index][state_index].find(element_index);
        if (result == m_mapping[instance_index][state_index].end()) {
            return nullptr;
        }
        return result.first->second;
    }
};


template<typename CONST_DENOTS>
struct DenotationsEqual {
public:
    bool operator()(const CONST_DENOTS& l, const CONST_DENOTS& r) const {
        return *l == *r;
    }
};


template<typename CONST_DENOTS>
struct DenotationsHasher {
public:
    std::size_t operator()(const CONST_DENOTS& denotations) const {
        // TODO
        return 0;
    }
};


/**
 * Caches a collection of denotation of type T.
 */
template<typename T>
class DenotationsCache {
private:
    using CONST_DENOTS = const std::vector<const T*>*;

    std::unordered_set<CONST_DENOTS, DenotationsHasher<CONST_DENOTS>, DenotationsEqual<CONST_DENOTS>> m_storage;

    // mapping from (element) -> std::vector<T*>*
    std::unordered_map<int, CONST_DENOTS> m_mapping;

    int m_num_states;
public:
    DenotationsCache(int num_states) : m_num_states(num_states) {}
    DenotationsCache(const DenotationsCache& other) = delete;
    DenotationsCache& operator=(const DenotationsCache& other) = delete;
    DenotationsCache(DenotationsCache&& other) = default;
    DenotationsCache& operator=(DenotationsCache&& other) = default;
    ~DenotationsCache() {
        for (auto denots : m_storage) {
            delete denots;
        }
    }

    std::vector<const T*>* get_new_denotations() const {
        auto result = new std::vector<const T*>();
        result->reserve(m_num_states);
        return result;
    }

    CONST_DENOTS insert(CONST_DENOTS denotations) {
        return m_storage.insert(denotations).first;
    }

    CONST_DENOTS find(int element_index) const {
        auto result = m_mapping.find(element_index);
        if (result == m_mapping.end())
            return nullptr;
        return result->second;
    }
};


struct DenotationsCaches {
    // Cache for single denotations.
    DenotationCache<bool> m_b_denot_cache;
    DenotationCache<int> m_n_denot_cache;
    DenotationCache<ConceptDenotation> m_c_denot_cache;
    DenotationCache<RoleDenotation> m_r_denot_cache;
    // Cache for collections of denotations.
    DenotationsCache<bool> m_b_denots_cache;
    DenotationsCache<int> m_n_denots_cache;
    DenotationsCache<ConceptDenotation> m_c_denots_cache;
    DenotationsCache<RoleDenotation> m_r_denots_cache;
};

}

#endif
