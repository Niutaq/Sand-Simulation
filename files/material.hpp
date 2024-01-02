#pragma once
#include "stuff.hpp"

class Material {

public:
    Material(std::string name, int id);
    void Gravity(std::vector<std::vector<int>> &grid, int i, int j); /// Metoda do zastosowania grawitacji 
    void UpdateMaterial(std::vector<std::vector<int>> &grid, int i, int j); /// Metoda aktualizacji materiału
    /// @see UpdateMaterialv2() UpdateMaterialv3() UpdateMaterialv4() UpdateMaterialv5()
    void UpdateMaterialv2(std::vector<std::vector<int>> &grid, int i, int j); /// Metoda aktualizacji materialu wody na siatce
    void UpdateMaterialv3(std::vector<std::vector<int>> &grid, int i, int j); /// Metoda aktualizacji materialu cząstek na siatce
    void UpdateMaterialv4(std::vector<std::vector<int>> &grid, int i, int j); /// Metoda aktualizacji materialu kwasu na siatce
    void UpdateMaterialv5(std::vector<std::vector<int>> &grid, int i, int j); /// Metoda aktualizacji materialu rośliny na siatce
    void Erase(std::vector<std::vector<int>> &grid, int i, int j); /// Metoda do usuwania materiału

private:
    std::string m_name; /// Nazwa materiału
    int m_id; /// Unikalny identyfikator materiału
};