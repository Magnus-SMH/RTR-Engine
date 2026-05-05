#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <initializer_list>
namespace RTR
{
    enum class ShaderDataType
    {
        Float2,
        Float3,
        Float4
    };

    inline uint32_t ShaderDataTypeSize(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float2: return 8;
            case ShaderDataType::Float3: return 12;
            case ShaderDataType::Float4: return 16;
        }
        return 0;
    }

    inline uint32_t ShaderDataTypeComponentCount(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float2: return 2;
            case ShaderDataType::Float3: return 3;
            case ShaderDataType::Float4: return 4;
        }
        return 0;
    }

    struct BufferElement
    {
        ShaderDataType type;
        std::string name;
        uint32_t offset;
        uint32_t size;
        bool normalized;

        BufferElement(ShaderDataType type, std::string name, bool normalized = false)
            : type(type),
            name(std::move(name)),
            offset(0),
            size(ShaderDataTypeSize(type)),
            normalized(normalized)
        {
        }
    };

    class BufferLayout
    {
    public:
        BufferLayout() = default;
        BufferLayout(std::initializer_list<BufferElement> elements)
            : m_Elements(elements)
        {
            CalculateOffsetsAndStride();
        }

        uint32_t GetStride() const { return m_Stride; }
        const std::vector<BufferElement>& GetElements() const { return m_Elements; }

    private:
        void CalculateOffsetsAndStride()
        {
            uint32_t offset = 0;
            for (auto& element : m_Elements)
            {
                element.offset = offset;
                offset += element.size;
            }
            m_Stride = offset;
        }
        std::vector<BufferElement> m_Elements;
        uint32_t m_Stride = 0;
    };

    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetLayout(const BufferLayout& layout) = 0;
        virtual const BufferLayout& GetLayout() const = 0;
        static std::shared_ptr<VertexBuffer> Create(const void* data, uint32_t size);
    };

    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() = default;
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        virtual uint32_t GetCount() const = 0;
        static std::shared_ptr<IndexBuffer> Create(const uint32_t* indices, uint32_t count);
    };
}