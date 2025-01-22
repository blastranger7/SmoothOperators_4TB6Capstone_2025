import requests
from kivy.app import App
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.textinput import TextInput
from kivy.uix.button import Button
from kivy.uix.label import Label

class CommandSender(BoxLayout):
    def __init__(self, **kwargs):
        super().__init__(orientation='vertical', **kwargs)

        # Input field
        self.input_field = TextInput(hint_text='Enter your commands as separate lines respectively (command, target distance, target angle):', multiline=True)
        self.add_widget(self.input_field)

        # Status label
        self.status_label = Label(text='Status: Idle')
        self.add_widget(self.status_label)

        # Send button
        self.send_button = Button(text='Send Command')
        self.send_button.bind(on_press=self.send_command)
        self.add_widget(self.send_button)

    def send_command(self, instance):
        url = "http://192.168.2.43:5000/write"  # Raspberry Pi's IP
        plain_text = self.input_field.text
        line_data = plain_text.splitlines()
        data = {"message": line_data}

        try:
            response = requests.post(url, json=data)
            if response.status_code == 200:
                self.status_label.text = f"Status: {response.json()['message']}"
            else:
                self.status_label.text = f"Status: Error - {response.json()['message']}"
        except Exception as e:
            self.status_label.text = f"Status: Error - {str(e)}"

class KivyClientApp(App):
    def build(self):
        return CommandSender()

if __name__ == '__main__':
    KivyClientApp().run()
