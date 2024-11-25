
def calculate_parity_bits(data_bits):
   
    p1 = data_bits[0] ^ data_bits[1] ^ data_bits[3]  
    p2 = data_bits[0] ^ data_bits[2] ^ data_bits[3]  
    p4 = data_bits[1] ^ data_bits[2] ^ data_bits[3]  

    
    return [p1, p2] + data_bits[:2] + [p4] + data_bits[2:]


def decode_hamming(code):
   
    p1 = code[0] ^ code[2] ^ code[4] ^ code[6]  
    p2 = code[1] ^ code[2] ^ code[5] ^ code[6]  
    p4 = code[3] ^ code[4] ^ code[5] ^ code[6]  
    
   
    error_position = p1 * 1 + p2 * 2 + p4 * 4
   
    if error_position != 0:
        print(f"Error at position {error_position}")
        
        code[error_position - 1] = 1 - code[error_position - 1]
    
   
    data_bits = [code[2], code[4], code[5], code[6]]
    
    return data_bits, code


def main():
    
    data_bits = []
    while len(data_bits) < 4:
        try:
            data_input = input("Enter 4 data bits (0 or 1): ")
            data_bits = [int(bit) for bit in data_input.strip()]
            if len(data_bits) != 4 or any(bit not in [0, 1] for bit in data_bits):
                print("Please enter exactly 4 bits (0 or 1).")
                data_bits = []
        except ValueError:
            print("Invalid input, please enter only 0s or 1s.")
            data_bits = []
    
    print(f"Original Data Bits: {data_bits}")
    
  
    encoded_code = calculate_parity_bits(data_bits)
    print(f"Encoded Hamming Code: {encoded_code}")
    
    
    introduce_error = input("Do you want to introduce an error in the encoded code? (y/n): ").strip().lower()
    if introduce_error == 'y':
        error_position = int(input(f"Enter the position (1-7) to introduce an error: "))
        if 1 <= error_position <= 7:
            encoded_code[error_position - 1] = 1 - encoded_code[error_position - 1]  # Flip the bit at the error position
            print(f"Encoded Code with Error: {encoded_code}")
        else:
            print("Invalid position, no error introduced.")
    
   
    decoded_data, corrected_code = decode_hamming(encoded_code)
    print(f"Decoded Data: {decoded_data}")
    print(f"Corrected Code: {corrected_code}")


if __name__ == "__main__":
    main()
