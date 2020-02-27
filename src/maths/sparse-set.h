#pragma once

#include <array>
#include <vector>
#include <cassert>
#include <glm/glm.hpp>

namespace voxmt {
    /**
     * @brief Store an array of data with O(1) access from (x, y, z) position
     */
    template<class T>
    class SparseSet {
    public:
        SparseSet() {}
        ~SparseSet() {}

        /**
         * @brief Insert or replace data at position
         */
        void insert(const glm::ivec3& position, const T& data) {
            // TODO assert max size
            // TODO position to sparse set id
        }

        /**
         * @brief Removes the data at position
         */
        void remove(const glm::ivec3& position) {
            // TODO get id at position
            // TODO switch last with removed to fill gaps
        }

        /**
         * @brief Check if data exist at this position
         */
        bool exist(const glm::ivec3& position) const {
            // TODO
        }

        /**
         * @brief Get the current number of data stored
         * @return size_t 
         */
        size_t size() const {
            return m_dense.size() - 1;
        }

        /**
         * @brief Get the max number of data to store
         * @return size_t 
         */
        size_t capacity() const {
            return m_sparse.size() - 1;
        }

        /**
         * @brief Get the data at the given position
         */
        T& at(const glm::ivec3& position) {
            assert(has(id) && "The entity does not have this component");
            return m_components.at(m_sparse.at(id));
        }

        // TODO get dense iterator in readonly
    
        // TODO handle sorting

    private:
        std::array<unsigned int, 1024> m_sparse;
        std::vector<T> m_dense;
    };
}
