import torch.nn as nn

# f1 = 9, f2 = 1, f3 = 5, n1 = 64, n2 = 32

class SRCNN(nn.Module):
    def __init__(self):
        super(SRCNN, self).__init__()
        
        self.layer1 = nn.Sequential(
                nn.Conv2d(3, 64, kernel_size = 9, stride = 1, padding = 4),
                nn.ReLU())
        
        self.layer2 = nn.Sequential(
                nn.Conv2d(64, 32, kernel_size = 1, stride = 1, padding = 0),
                nn.ReLU())
        
        self.layer3 = nn.Sequential(
                nn.Conv2d(32, 3, kernel_size = 5, stride = 1, padding = 2))
        
    def forward(self, x):
        
        x = self.layer1(x)
        x = self.layer2(x)
        x = self.layer3(x)
        
        return x